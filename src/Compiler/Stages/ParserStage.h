#pragma once
#include "IStage.h"
#include "LexerStage.h"
#include "CompilerParts/Observers/INewLexerTokenObserver.h"

class ParserStage : public IStage, public INewLexerTokenObserver
{
public:
	~ParserStage() override {}
	void DoStage() override;

	/// INewLexerTokenObserver override
	void Notify(LexerStage::Token token);
	///

	std::wstring GetStageName() override { return L"ParserStage"; }

	std::vector<LexerStage::Token> m_lexerStageTokens;

protected:
	void Clear();
};