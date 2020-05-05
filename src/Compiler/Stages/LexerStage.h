#pragma once
#include "CompilerParts/StageOutputStructs.h"
#include "IStage.h"
#include "CompilerParts/Observers/IObservable.h"

class __declspec(dllexport) LexerStage : public IStage, public IObservable<IObserver<const Token&>, const Token&>
{
public:
	LexerStage() = delete;
	LexerStage(const std::wstring& runningDirectory, std::wstring file, bool needLog);
	~LexerStage() override = default;

	/// IStage override
	void DoStage() override;

	[[nodiscard]] std::wstring GetStageName() override { return L"Lexer"; }
	///

	struct TokenRule
	{
		TokenRule() : NeedAdditionalCheck(false) {};
		std::wregex  Regexpr;
		std::wstring Type;
		bool		 NeedAdditionalCheck;
		std::wregex	 AdditionalCheck;
	};

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
};