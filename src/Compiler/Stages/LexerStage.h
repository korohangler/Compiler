#pragma once
#include "IStage.h"

class LexerStage : public IStage
{
public:
	~LexerStage() override {}

	void DoStage(const std::wistream& inputStream, std::wostream& outputStream) override;

	std::wstring GetStageName() override { return L"Lexer"; };

protected:
	std::regex m_separatorRegexp;
};