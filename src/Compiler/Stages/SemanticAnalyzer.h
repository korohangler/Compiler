#pragma once
#include "IStage.h"
#include "CompilerParts/StageOutputStructs.h"
#include "CompilerParts/Observers/IObservable.h"

class __declspec(dllexport) SemanticAnalyzer : public IStage, public IObservable<IObserver<std::shared_ptr<AbstractTreeNode>>, std::shared_ptr<AbstractTreeNode>>, public IObserver<std::shared_ptr<AbstractTreeNode>>
{
public:
	~SemanticAnalyzer() override = default;
	/// IStage override
	void DoStage() override;

	[[nodiscard]] std::wstring GetStageName() override { return L"CodeGenerator"; }
	///

	/// IObserver override
	void Notify(std::shared_ptr<AbstractTreeNode> root) override;
	///
	
private:

	struct ScopeData
	{
		size_t level = 0;
		size_t counter = 0;

		std::wstring ToString() const;
	};

	void ProcessNode(std::shared_ptr<AbstractTreeNode> node, const ScopeData& currScopeData);
};
