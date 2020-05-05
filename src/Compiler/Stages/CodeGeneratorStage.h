#pragma once
#include "IStage.h"
#include "CompilerParts/Observers/INewParserTreeObserver.h"

class __declspec(dllexport) CodeGeneratorStage : public IStage, public INewParserTreeObserver
{
public:
	void DoStage() override;

	[[nodiscard]] std::wstring GetStageName() override { return L"CodeGenerator"; }

	void Notify(const AbstractTreeNode* root) override;
};