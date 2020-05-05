#include "stdafx.h"
#include "LexerStage.h"

LexerStage::LexerStage(const std::wstring& runningDirectory, std::wstring file, bool needLog) : m_needLog(needLog), m_inputFileName(std::move(file))
{
	std::wstring pathToConfig = runningDirectory + L"../../../data/Lexer/regexps.json";

	std::wifstream rulesFile(pathToConfig);
	
	ASSERT2(rulesFile.is_open(), "Unable to open Lexer config file!");

	rapidjson::WIStreamWrapper isw(rulesFile);

	WDocument document;
	document.ParseStream(isw);

	InitRules(document);
}

void LexerStage::DoStage()
{
	std::wifstream input(m_inputFileName);
	ReadText(input);

	while (m_currTextPos < m_inputText.end())
	{
		const Token newToken = ParseToken();

		Notify(newToken);
	}

	Notify(FinalToken);

	Clear();
}

void LexerStage::ReadText(std::wistream& inputStream)
{
	if (m_needLog) Utils::Logger::Log("LEXER: Reading text");
	m_inputText   = std::wstring(std::istreambuf_iterator<wchar_t>(inputStream), std::istreambuf_iterator<wchar_t>());
	m_currTextPos = m_inputText.begin();
	if (m_needLog) Utils::Logger::Log("LEXER: Reading text completed!\n");
}

Token LexerStage::ParseToken()
{
	std::wcmatch match;

	bool additionalCheckFailed = false;

	const auto rule = std::find_if(m_tokenRules.begin(), m_tokenRules.end(), [&](TokenRule& currRule) {
		const bool res = std::regex_search(&(*m_currTextPos),
			   match,
			   currRule.Regexpr,
			   std::regex_constants::match_continuous); 

		if (currRule.NeedAdditionalCheck && res)
			additionalCheckFailed = !std::regex_match(match.str(0), currRule.AdditionalCheck);

		return res;
	});

	if (rule == m_tokenRules.end() || additionalCheckFailed)
	{
		ASSERT(std::string("Cannot recognize token at line: ") + std::to_string(std::count(m_inputText.begin(), m_currTextPos, L'\n')));
	}

	const std::wstring res = match.str(0);

	m_currTextPos += res.length();

	return Token(res, rule->Type, std::count(m_inputText.begin(), m_currTextPos, L'\n'));
}

void LexerStage::Clear()
{
	m_inputText.clear();
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

			newRule.NeedAdditionalCheck = rule.HasMember(L"NeedAdditionalCheck") ? rule[L"NeedAdditionalCheck"].GetBool() : false;
			newRule.AdditionalCheck	    = rule.HasMember(L"AdditionalCheck")	 ? rule[L"AdditionalCheck"].GetString()   : L"";
			
			m_tokenRules.emplace_back(std::move(newRule));
		}
	}
}
