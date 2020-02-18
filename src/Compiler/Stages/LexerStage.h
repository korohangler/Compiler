#pragma once
#include "IStage.h"

class LexerStage : public IStage
{
public:
	LexerStage(std::wstring runningDirectory);
	~LexerStage() override {}

	void DoStage(std::wistream& inputStream, std::wostream& outputStream) override;

	std::wstring GetStageName() override { return L"Lexer"; };

	struct TokenRule
	{
		std::wregex  Regexpr;
		std::wstring Type;
	};

	struct Token
	{
		Token() {};
		Token(std::wstring val, std::wstring type) : Value(val), Type(type) {};

		std::wstring Value;
		std::wstring Type;
	};

protected:
	void ReadText(std::wistream& inputStream);
	void ParseText();
	void SaveTokens(std::wostream& outputStream);
	void InitRules(rapidjson::Document& doc);

	std::vector<TokenRule>	  m_tokenRules;
	std::vector<Token>		  m_tokens;
	std::wstring m_inputText;
};