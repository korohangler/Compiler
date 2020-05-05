#pragma once
#include "CompilerParts/StageOutputStructs.h"
#include "IStage.h"

class INewLexerTokenObserver;

class __declspec(dllexport) LexerStage : public IStage
{
public:
	LexerStage() = delete;
	LexerStage(std::wstring runningDirectory, const std::wstring& file, bool needLog);
	~LexerStage() override {}

	void DoStage() override;

	[[nodiscard]] std::wstring GetStageName() override { return L"Lexer"; };

	struct TokenRule
	{
		TokenRule() : NeedAdditionalCheck(false) {};
		std::wregex  Regexpr;
		std::wstring Type;
		bool		 NeedAdditionalCheck;
		std::wregex	 AdditionalCheck;
	};

	void RegisterListener(INewLexerTokenObserver* observer) { m_observers.push_back(observer); }
	void UnRegisterListener(INewLexerTokenObserver* observer) { m_observers.erase(std::find(m_observers.begin(), m_observers.end(), observer)); }

	static inline const Token FinalToken = Token(L"FINAL", L"FINAL", -1);
	
protected:

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