#pragma once
#include "Stages/LexerStage.h"
#include "IObserver.h"

class INewLexerTokenObserver : public IObserver
{
public:
	virtual void Notify(LexerStage::Token) = 0;
};