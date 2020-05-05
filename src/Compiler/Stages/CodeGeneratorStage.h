#pragma once
#include "CompilerParts/IdentificatorTable.h"
#include "IStage.h"
#include "CompilerParts/Observers/IObserver.h"
#include "CompilerParts/StageOutputStructs.h"

class __declspec(dllexport) CodeGeneratorStage : public IStage, public IObserver<std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>>>
{
public:
	/// IStage override
	void DoStage() override;

	[[nodiscard]] std::wstring GetStageName() override { return L"CodeGenerator"; }
	///

	/// INewParserTreeObserver override
	void Notify(std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>> data) override;
	///
};
