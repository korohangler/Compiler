#include "stdafx.h"
#include "SemanticAnalyzer.h"
#include "ParserElements/Scope.h"
#include "CompilerParts/IdentificatorTable.h"
#include "ParserElements/ExpressionStatement.h"
#include "ParserElements/Function.h"
#include "ParserElements/Identificator.h"
#include "ParserElements/Let.h"

void SemanticAnalyzer::DoStage()
{
}

void SemanticAnalyzer::Notify(std::shared_ptr<AbstractTreeNode> root)
{
	const ScopeData rootScopeData = {0, 0};

	ProcessNode(root, rootScopeData);

	for (auto& observer : m_observers) observer->Notify(root);
}

void SemanticAnalyzer::ProcessNode(std::shared_ptr<AbstractTreeNode> node, const ScopeData& currScopeData)
{
	const auto scope = std::dynamic_pointer_cast<Scope>(node);
	const auto identificator = std::dynamic_pointer_cast<Identificator>(node);
	const auto let = std::dynamic_pointer_cast<Let>(node);
	const auto function = std::dynamic_pointer_cast<Function>(node);
	const auto expressionStatement = std::dynamic_pointer_cast<ExpressionStatement>(node);

	if (scope != nullptr)
	{
		const ScopeData newScopeData = { currScopeData.level + 1, 0 };
		
		IdentificatorTable::GetInstance().AddScope(currScopeData.ToString(), newScopeData.ToString());

		for (auto& child : node->m_childs) ProcessNode(child, newScopeData);
	}
	else if(identificator != nullptr)
	{
		ASSERT2(IdentificatorTable::GetInstance().IsIdentificatorExist(currScopeData.ToString(), identificator->GetVariableName()),
			std::wstring(L"Variable: ") + identificator->GetVariableName() + std::wstring(L". Does not exist"));

		for (auto& child : node->m_childs) ProcessNode(child, currScopeData);
	}
	else if(let != nullptr)
	{
		IdentificatorTable::GetInstance().AddIdentificator(currScopeData.ToString(), let->GetVariableName());
		
		for (auto& child : node->m_childs) ProcessNode(child, currScopeData);
	}
	else if(function != nullptr)
	{
		IdentificatorTable::GetInstance().AddIdentificator(currScopeData.ToString(), function->GetFunctionName());

		for (auto& child : node->m_childs) ProcessNode(child, currScopeData);
	}
	else if(expressionStatement != nullptr)
	{
		ASSERT2(IdentificatorTable::GetInstance().IsIdentificatorExist(currScopeData.ToString(), expressionStatement->GetIdentificatorName()),
			std::wstring(L"Use of undefined variable: ") + expressionStatement->GetIdentificatorName());

		for (auto& child : node->m_childs) ProcessNode(child, currScopeData);
	}
	else
	{
		for (auto& child : node->m_childs) ProcessNode(child, currScopeData);
	}
}

std::wstring SemanticAnalyzer::ScopeData::ToString() const
{
	return std::wstring(L"l") + std::to_wstring(this->level) + std::wstring(L"m") + std::to_wstring(this->counter);
}
