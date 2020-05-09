#pragma once
#include "CompilerParts/IdentificatorTable.h"
#include "IStage.h"
#include "CompilerParts/StageOutputStructs.h"
#include "CompilerParts/Observers/Observable.h"

class __declspec(dllexport) SemanticAnalyzer : public IStage,
	public Observable<std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>>>,
	public IObserver<std::shared_ptr<AbstractTreeNode>>
{
public:
	SemanticAnalyzer() : m_identificatorTable(std::make_shared<IdentificatorTable>()) {}
	~SemanticAnalyzer() override = default;
	/// IStage override
	void DoStage() override {}

	[[nodiscard]] std::wstring GetStageName() override { return L"CodeGenerator"; }
	///

	/// IObserver override
	void Notify(std::shared_ptr<AbstractTreeNode> root) override;
	///
	
private:
	std::shared_ptr<IdentificatorTable> m_identificatorTable;

	struct ScopeData
	{
		size_t level = 0;
		size_t counter = 0;

		[[nodiscard]] std::wstring ToString() const;
	};

	void ProcessNode(std::shared_ptr<AbstractTreeNode> node, const ScopeData& currScopeData, size_t childIdx);
};
