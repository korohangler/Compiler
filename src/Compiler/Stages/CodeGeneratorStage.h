#pragma once
#include "IStage.h"
#include "CompilerParts/Observers/INewParserTreeObserver.h"

class CodeGeneratorStage : public IStage, public INewParserTreeObserver
{
public:
	void DoStage() override;

	std::wstring GetStageName() override { return L"CodeGenerator"; }

	void Notify(const AbstractTreeNode* root) override;
};