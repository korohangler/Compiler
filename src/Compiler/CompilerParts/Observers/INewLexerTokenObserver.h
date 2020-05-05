#pragma once
#include "Stages/LexerStage.h"
#include "IObserver.h"
#include "CompilerParts/StageOutputStructs.h"

class __declspec(dllexport) INewLexerTokenObserver : public IObserver
{
public:
	virtual ~INewLexerTokenObserver() = default;
	
	virtual void Notify(const Token&) = 0;
};