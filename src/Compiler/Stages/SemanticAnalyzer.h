#pragma once
#include "IStage.h"
#include "CompilerParts/Observers/INewParserTreeObserver.h"

class SemanticAnalyzer : public IStage, public INewParserTreeObserver
{
public:
	void DoStage() override;

	std::wstring GetStageName() override { return L"CodeGenerator"; }

	void Notify(const AbstractTreeNode* root) override;

	void RegisterListener(INewParserTreeObserver* observer) { m_observers.push_back(observer); }
	void UnRegisterListener(INewParserTreeObserver* observer) { m_observers.erase(std::find(m_observers.begin(), m_observers.end(), observer)); }
	
private:

	struct ScopeData
	{
		size_t level = 0;
		size_t counter = 0;

		std::wstring ToString() const;
	};

	void ProcessNode(const AbstractTreeNode* node, const ScopeData& currScopeData);
	
	std::vector<INewParserTreeObserver*> m_observers;
};
