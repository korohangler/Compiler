#pragma once
#include "Stages/LexerStage.h"
#include "IObserver.h"

class INewLexerTokenObserver : public IObserver
{
public:
	virtual ~INewLexerTokenObserver() = default;
	
	virtual void Notify(const Token&) = 0;
};