#include "stdafx.h"
#include "LexerStage.h"

LexerStage::LexerStage(std::wstring runningDirectory)
{
	std::wstring pathToConfig = runningDirectory + L"../../../data/Lexer/regexps.json";

	std::wifstream rulesFile(pathToConfig);
	
	ASSERT2(rulesFile.is_open(), "Unable to open Lexer config file!");

	rapidjson::WIStreamWrapper isw(rulesFile);

	WDocument document;
	document.ParseStream(isw);

	InitRules(document);
}

void LexerStage::DoStage(std::wistream& inputStream, std::wostream& outputStream)
{
	ReadText(inputStream);
	ParseText();
	OptimizeTokens();
	SaveTokens(outputStream);
	Clear();
}

std::wstring LexerStage::DoStage(std::wstring& inputString)
{
	std::wstringstream input(inputString);
	std::wstringstream output;
	DoStage(input, output);
	return output.str();
}

void LexerStage::ReadText(std::wistream& inputStream)
{
	m_inputText   = std::wstring(std::istreambuf_iterator<wchar_t>(inputStream), std::istreambuf_iterator<wchar_t>());
	m_currTextPos = m_inputText.begin();
}

void LexerStage::ParseText()
{
	while (m_currTextPos < m_inputText.end())
		ASSERT2(ApplyRules(), std::string("Cannot recognize token at line: ") + std::to_string(std::count(m_inputText.begin(), m_currTextPos, L'\n')));
}

bool LexerStage::ApplyRules()
{
	std::wsmatch match;

	auto rule = std::find_if(m_tokenRules.begin(), m_tokenRules.end(), [&](TokenRule& currRule) {
		return std::regex_search(std::wstring::const_iterator(m_currTextPos),
			   m_inputText.cend(),
			   match,
			   currRule.Regexpr,
			   std::regex_constants::match_continuous); 
	});

	if (rule == m_tokenRules.end()) return false;

	if (rule->NeedExport
		&& (!rule->DeleteDuplicates
			|| m_tokens.size() > 0
			&& m_tokens.back().Value != match.str(0)))
	{
		m_tokens.emplace_back(match.str(0), rule->Type);
	}

	m_currTextPos += match.str(0).length();

	return true;
}

void LexerStage::OptimizeTokens()
{
	for (size_t i = 0; i < m_tokens.size() - 1; i++)
	{
		auto rule = std::find_if(m_optimizationRules.begin(), m_optimizationRules.end(), [&](ExportOptimizationRule& rule) {
			return m_tokens[i].Type == rule.Type
				&& m_tokens[i].Type == m_tokens[i + 1].Type
				&& (m_tokens[i].Value + m_tokens[i + 1].Value == rule.From); 
		});

		if (rule == m_optimizationRules.end()) continue;

		m_tokens[i].Value = rule->To;
		m_tokens.erase(m_tokens.begin() + i + 1);
		i--;
	}
}

void LexerStage::SaveTokens(std::wostream& outputStream)
{
	WDocument doc;
	doc.SetObject();

	WValue myArray(rapidjson::kArrayType);
	WDocument::AllocatorType& allocator = doc.GetAllocator();

	for (auto& token : m_tokens)
	{
		WValue objValue;
		objValue.SetObject();

		WValue tmp;
		tmp.SetString(token.Type.c_str(), static_cast<rapidjson::SizeType>(token.Type.length()));
		objValue.AddMember(L"Type", tmp, allocator);

		tmp.SetString(token.Value.c_str(), static_cast<rapidjson::SizeType>(token.Value.length()));
		objValue.AddMember(L"Value", tmp, allocator);

		myArray.PushBack(objValue, allocator);
	}

	doc.AddMember(L"Tokens", myArray, allocator);
	WStringBuffer strbuf;
	rapidjson::Writer<WStringBuffer, rapidjson::UTF16<>> writer(strbuf);
	doc.Accept(writer);

	std::wstring result = strbuf.GetString();

	outputStream.write(result.c_str(), result.length());
}

void LexerStage::Clear()
{
	m_inputText.clear();
	m_tokens.clear();
	m_currTextPos = m_inputText.end();
}

void LexerStage::InitRules(WDocument& doc)
{
	if (doc.HasMember(L"Rules"))
	{
		auto rules = doc[L"Rules"].GetArray();

		for (auto& rule : rules)
		{
			TokenRule newRule;
			newRule.Regexpr = rule.HasMember(L"Regexp") ? rule[L"Regexp"].GetString() : L"";
			newRule.Type    = rule.HasMember(L"Type")   ? rule[L"Type"].GetString()   : L"";

			newRule.NeedExport       = rule.HasMember(L"NeedExport")       ? rule[L"NeedExport"].GetBool()       : true;
			newRule.DeleteDuplicates = rule.HasMember(L"DeleteDuplicates") ? rule[L"DeleteDuplicates"].GetBool() : false;

			m_tokenRules.emplace_back(newRule);
		}
	}

	if (doc.HasMember(L"Optimize"))
	{
		const auto& optimizationRules = doc[L"Optimize"].GetArray();

		for (const auto& optRule : optimizationRules)
		{
			ExportOptimizationRule newRule;
			newRule.Type = optRule.HasMember(L"Type") ? optRule[L"Type"].GetString() : L"InvalidType";
			newRule.From = optRule.HasMember(L"From") ? optRule[L"From"].GetString() : L"";
			newRule.To   = optRule.HasMember(L"To")   ? optRule[L"To"].GetString()   : L"";

			m_optimizationRules.emplace_back(newRule);
		}
	}
}
