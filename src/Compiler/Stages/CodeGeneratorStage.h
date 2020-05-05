#pragma once
#include "IStage.h"
#include "CompilerParts/Observers/IObservable.h"
#include "CompilerParts/StageOutputStructs.h"

class __declspec(dllexport) CodeGeneratorStage : public IStage, public IObserver<std::shared_ptr<AbstractTreeNode>>
{
public:
	/// IStage override
	void DoStage() override;

	[[nodiscard]] std::wstring GetStageName() override { return L"CodeGenerator"; }
	///

	/// INewParserTreeObserver override
	void Notify(std::shared_ptr<AbstractTreeNode> root) override;
	///
};
