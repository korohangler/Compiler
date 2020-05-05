#pragma once
#include "IObserver.h"
#include "CompilerParts/StageOutputStructs.h"

class __declspec(dllexport) INewParserTreeObserver : public IObserver
{
public:
	virtual ~INewParserTreeObserver() = default;
	
	virtual void Notify(const AbstractTreeNode* root) = 0;
};