#include "stdafx.h"
#include "SemanticAnalyzer.h"
#include "ParserElements/Scope.h"
#include "CompilerParts/IdentificatorTable.h"
#include "ParserElements/ExpressionStatement.h"
#include "ParserElements/Function.h"
#include "ParserElements/Identificator.h"
#include "ParserElements/Let.h"
#include "Stages\ParserElements\Literal.h"

void SemanticAnalyzer::Notify(std::shared_ptr<AbstractTreeNode> root)
{
	const ScopeData rootScopeData = {0, 0};

	ProcessNode(root, rootScopeData, 0);

	NotifyListeners(std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>>(root, m_identificatorTable));
}

void SemanticAnalyzer::ProcessNode(std::shared_ptr<AbstractTreeNode> node, const ScopeData& currScopeData, size_t childIdx)
{
	const auto scope = std::dynamic_pointer_cast<Scope>(node);
	const auto identificator = std::dynamic_pointer_cast<Identificator>(node);
	const auto let = std::dynamic_pointer_cast<Let>(node);
	const auto function = std::dynamic_pointer_cast<Function>(node);
	const auto expressionStatement = std::dynamic_pointer_cast<ExpressionStatement>(node);
	const auto liter = std::dynamic_pointer_cast<Literal>(node);

	if (scope != nullptr)
	{
		const ScopeData newScopeData = { currScopeData.level + 1, childIdx };
		
		m_identificatorTable->AddScope(currScopeData.ToString(), newScopeData.ToString());

		for (size_t i = 0; i < node->Childs.size(); i++) 
			ProcessNode(node->Childs[i], newScopeData, i);
	}
	else if(identificator != nullptr)
	{
		Utils::ASSERT2(m_identificatorTable->IsIdentificatorExist(currScopeData.ToString(), identificator->GetVariableName()),
		               std::wstring(L"Variable: ") + std::wstring(identificator->GetVariableName()) + std::wstring(L". Does not exist"));

		identificator->SetAttribute(L"PositionOnStack", 
			std::to_wstring(m_identificatorTable->GetIdentificatorInfo(currScopeData.ToString(), identificator->GetVariableName()).VariableLocationOnStack));

		for (size_t i = 0; i < node->Childs.size(); i++)
			ProcessNode(node->Childs[i], currScopeData, i);
	}
	else if(let != nullptr)
	{
		IdentificatorInfo info;
		info.AsmName = std::wstring(L"Var") + std::to_wstring(m_counterForVariableName);
		m_counterForVariableName++;

		info.VariableLocationOnStack = m_positionOnStack;
		m_positionOnStack++;

		m_identificatorTable->AddIdentificator(currScopeData.ToString(), let->GetVariable()->GetVariableName(), info);
		
		for (size_t i = 0; i < node->Childs.size(); i++)
			ProcessNode(node->Childs[i], currScopeData, i);
	}
	else if(function != nullptr)
	{
		IdentificatorInfo info;
		info.AsmName = std::wstring(L"Function") + std::to_wstring(m_counterForFunctionName);
		m_counterForFunctionName++;

		info.VariableLocationOnStack = -1;

		m_identificatorTable->AddIdentificator(currScopeData.ToString(), function->GetFunctionName(), info);

		for (size_t i = 0; i < node->Childs.size(); i++)
			ProcessNode(node->Childs[i], currScopeData, i);
	}
	else if(expressionStatement != nullptr)
	{
		Utils::ASSERT2(m_identificatorTable->IsIdentificatorExist(currScopeData.ToString(), expressionStatement->GetIdentificator()->GetVariableName()),
		               std::wstring(L"Use of undefined variable: ") + std::wstring(expressionStatement->GetIdentificator()->GetVariableName()));

		for (size_t i = 0; i < node->Childs.size(); i++)
			ProcessNode(node->Childs[i], currScopeData, i);
	}
	else if (liter != nullptr)
	{
		liter->SetAttribute(L"PositionOnStack", std::to_wstring(m_positionOnStack));
		m_positionOnStack++;

		for (size_t i = 0; i < node->Childs.size(); i++)
			ProcessNode(node->Childs[i], currScopeData, i);
	}
	else
	{
		for (size_t i = 0; i < node->Childs.size(); i++)
			ProcessNode(node->Childs[i], currScopeData, i);
	}
}

std::wstring SemanticAnalyzer::ScopeData::ToString() const
{
	return std::wstring(L"l") + std::to_wstring(this->level) + std::wstring(L"m") + std::to_wstring(this->counter);
}
