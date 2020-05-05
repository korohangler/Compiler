#pragma once
#include <vector>

#include "../Compiler/CompilerParts/Observers/INewLexerTokenObserver.h"

class LexerTester : public INewLexerTokenObserver
{
public:
	~LexerTester() override = default;
	LexerTester(const std::vector<Token>& expectedTokens);

	void Notify(const Token& token) override;

private:

	const std::vector<Token>& m_expectedTokens;

	size_t m_currToken;
};