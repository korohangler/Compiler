#pragma once
#include "IStage.h"

class LexerStage : public IStage
{
public:
	LexerStage() {}
	LexerStage(std::wstring runningDirectory);
	~LexerStage() override {}

	void DoStage(std::wistream& inputStream, std::wostream& outputStream) override;
	std::wstring DoStage(std::wstring& inputString);

	std::wstring GetStageName() override { return L"Lexer"; };

	struct TokenRule
	{
		TokenRule() : DeleteDuplicates(false), NeedExport(true){};
		TokenRule(std::wstring regExp, std::wstring type, bool delDupl, bool needExp) : Regexpr(regExp), Type(type), DeleteDuplicates(delDupl), NeedExport(needExp) {}
		std::wregex  Regexpr;
		std::wstring Type;
		bool		 DeleteDuplicates;
		bool		 NeedExport;
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
	void Clear();
	void InitRules(WDocument& doc);

	std::vector<TokenRule>	  m_tokenRules;
	std::vector<Token>		  m_tokens;
	std::wstring m_inputText;
};