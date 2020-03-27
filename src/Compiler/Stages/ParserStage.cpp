#include "stdafx.h"
#include "ParserStage.h"

void ParserStage::DoStage()
{
	Clear();
}

void ParserStage::Notify(LexerStage::Token token)
{
}

void ParserStage::Clear()
{
	m_lexerStageTokens.clear();
}
