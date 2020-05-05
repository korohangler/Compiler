#pragma once
#include <vector>
#include "CompilerParts/Observers/IObserver.h"
#include "CompilerParts/StageOutputStructs.h"

class LexerTester : public IObserver<const Token&>
{
public:
	~LexerTester() override = default;
	LexerTester(const std::vector<Token>& expectedTokens);

	/// IObserver override
	void Notify(const Token& token) override;
	///

private:

	const std::vector<Token>& m_expectedTokens;

	size_t m_currToken;
};
