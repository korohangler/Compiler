#pragma once
#include "IStage.h"

class INewLexerTokenObserver;

class LexerStage : public IStage
{
public:
	LexerStage(std::wstring runningDirectory, const std::wstring& file, bool needLog);
	~LexerStage() override {}

	void DoStage() override;

	std::wstring GetStageName() override { return L"Lexer"; };

	struct TokenRule
	{
		TokenRule() : NeedAdditionalCheck(false) {};
		std::wregex  Regexpr;
		std::wstring Type;
		bool		 NeedAdditionalCheck;
		std::wregex	 AdditionalCheck;
	};

	struct Token
	{
		Token() : Line(0) {};
		Token(std::wstring val, std::wstring type, size_t line) : Value(val), Type(type), Line(line) {};

		std::wstring Value;
		std::wstring Type;
		size_t Line;
	};

	void RegisterListener(INewLexerTokenObserver* observer) { m_observers.push_back(observer); }
	void UnRegisterListener(INewLexerTokenObserver* observer) { m_observers.erase(std::find(m_observers.begin(), m_observers.end(), observer)); }

protected:
	LexerStage();

	void  ReadText(std::wistream& inputStream);
	Token ParseToken();
	void  Clear();
	void  InitRules(WDocument& doc);

	bool m_needLog;

	std::vector<TokenRule> m_tokenRules;
	std::wstring		   m_inputText;
	std::wstring::iterator m_currTextPos;
	std::wstring		   m_inputFileName;

	std::vector<INewLexerTokenObserver*> m_observers;
};