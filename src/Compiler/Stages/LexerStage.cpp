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
	m_inputText = std::wstring(std::istreambuf_iterator<wchar_t>(inputStream), std::istreambuf_iterator<wchar_t>());
}

void LexerStage::ParseText()
{
	size_t currCharIdx = 0;

	size_t currentLine = 0;

	std::wstring::iterator currCharacter = m_inputText.begin() + currCharIdx;

	while (currCharacter != m_inputText.end())
	{
		currCharacter = m_inputText.begin() + currCharIdx;

		bool foundCorrectType = false;

		std::wsmatch resultedmatch;

		for (size_t i = 0; i < m_tokenRules.size(); i++)
		{
			if (std::regex_search(std::wstring::const_iterator(currCharacter), m_inputText.cend(), resultedmatch, m_tokenRules[i].Regexpr)
				&& resultedmatch.position(0) == 0)
			{
				foundCorrectType = true;

				std::wstring resultedString = resultedmatch.str(0);

				size_t len = resultedString.length();

				currCharIdx += len;

				for (auto& iter : resultedString) if (iter == L'\n') currentLine++;

				if (!m_tokenRules[i].NeedExport) break;
				if (m_tokens.size() > 0 && m_tokenRules[i].DeleteDuplicates && m_tokens.back().Value == resultedString && m_tokens.back().Type == m_tokenRules[i].Type) break;

				m_tokens.emplace_back(resultedString, m_tokenRules[i].Type);

				break;
			}
		}

		ASSERT2(foundCorrectType, std::wstring(L"Cannot recognize token at line: ") + std::to_wstring(currentLine));
	}
}

void LexerStage::OptimizeTokens()
{
	for (size_t i = 0; i < m_tokens.size() - 1; i++)
	{
		for (size_t j = 0; j < m_optimizationRules.size(); j++)
		{
			if (m_tokens[i].Type == m_optimizationRules[j].Type 
				&& m_tokens[i].Type == m_tokens[i + 1].Type 
				&& (m_tokens[i].Value + m_tokens[i + 1].Value == m_optimizationRules[j].From))
			{
				m_tokens[i].Value = m_optimizationRules[j].To;
				m_tokens.erase(m_tokens.begin() + i + 1);
				i--;
				break;
			}
		}
	}
}

void LexerStage::SaveTokens(std::wostream& outputStream)
{
	WDocument doc;
	doc.SetObject();

	WValue myArray(rapidjson::kArrayType);
	WDocument::AllocatorType& allocator = doc.GetAllocator();

	for (size_t i = 0; i < m_tokens.size(); i++)
	{
		WValue objValue;
		objValue.SetObject();

		WValue tmp;
		tmp.SetString(m_tokens[i].Type.c_str(), static_cast<rapidjson::SizeType>(m_tokens[i].Type.length()));
		objValue.AddMember(L"Type", tmp, allocator);

		tmp = WValue();
		tmp.SetString(m_tokens[i].Value.c_str(), static_cast<rapidjson::SizeType>(m_tokens[i].Value.length()));
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
}

void LexerStage::InitRules(WDocument& doc)
{
	auto rules = doc[L"Rules"].GetArray();

	for (auto i = rules.Begin(); i < rules.End(); i++)
	{
		TokenRule newRule;
		newRule.Regexpr = std::wregex((*i)[L"Regexp"].GetString(), std::regex_constants::ECMAScript);
		newRule.Type	= (*i)[L"Type"].GetString();

		newRule.NeedExport = (*i).HasMember(L"NeedExport") ? (*i)[L"NeedExport"].GetBool() : true;
		newRule.DeleteDuplicates = (*i).HasMember(L"DeleteDuplicates") ? (*i)[L"DeleteDuplicates"].GetBool() : false;

		m_tokenRules.emplace_back(newRule);
	}

	if (doc.HasMember(L"Optimize"))
	{
		const auto& optimizationRules = doc[L"Optimize"];

		for (auto i = optimizationRules.Begin(); i < optimizationRules.End(); i++)
		{
			ExportOptimizationRule newRule;
			newRule.Type = (*i).HasMember(L"Type") ? (*i)[L"Type"].GetString() : L"InvalidType";
			newRule.From = (*i).HasMember(L"From") ? (*i)[L"From"].GetString() : L"";
			newRule.To   = (*i).HasMember(L"To")   ? (*i)[L"To"].GetString()   : L"";

			m_optimizationRules.emplace_back(newRule);
		}
	}
}
