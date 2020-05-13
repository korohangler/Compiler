#include "stdafx.h"
#include "CodeGeneratorStage.h"


#include "ParserElements/ExpressionStatement.h"
#include "ParserElements/Function.h"
#include "ParserElements/Identificator.h"
#include "ParserElements/If.h"
#include "ParserElements/Let.h"
#include "ParserElements/Literal.h"
#include "ParserElements/Scope.h"
#include "ParserElements/While.h"
#include "ParserElements/Expression.h"
#include "ParserElements/Identificator.h"
#include "Stages\ParserElements\MathOperations\Operation.h"

void CodeGeneratorStage::Notify(std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>> data)
{
	m_table = data.second;

	TranslateNode(data.first);

	NotifyListeners(m_constructor);
}

void CodeGeneratorStage::TranslateNode(std::shared_ptr<AbstractTreeNode> node)
{
	auto let = std::dynamic_pointer_cast<Let>(node);
	auto expressionStatement = std::dynamic_pointer_cast<ExpressionStatement>(node);
	auto ifElem = std::dynamic_pointer_cast<If>(node);
	auto whileElem = std::dynamic_pointer_cast<While>(node);
	auto scope = std::dynamic_pointer_cast<Scope>(node);
	auto function = std::dynamic_pointer_cast<Function>(node);

	if (let != nullptr)
		TranslateLet(let);
	else if (expressionStatement != nullptr)
		TranslateExpressionStatement(expressionStatement);
	else if (ifElem != nullptr)
		TranslateIfElement(ifElem);
	else if (whileElem != nullptr)
		TranslateWhileElement(whileElem);
	else if (scope != nullptr)
		for (auto child : scope->Childs) TranslateNode(child);
	else if (function != nullptr)
		TranslateFunction(function);
}

void CodeGeneratorStage::TranslateLet(std::shared_ptr<Let> node)
{
	// TODO For now no initialization at all
	m_constructor->AddVariable(node->GetVariable()->GetVariableName());
}

void CodeGeneratorStage::TranslateExpressionStatement(std::shared_ptr<ExpressionStatement> node)
{
	// TODO functions

	TranslateExpression(node->GetExpression(), node->GetIdentificator());
}

void CodeGeneratorStage::TranslateIfElement(std::shared_ptr<If> node)
{
}

void CodeGeneratorStage::TranslateWhileElement(std::shared_ptr<While> node)
{
}

void CodeGeneratorStage::TranslateFunction(std::shared_ptr<Function> node)
{
}

void CodeGeneratorStage::TranslateExpression(std::shared_ptr<Expression> node, std::shared_ptr<Identificator> whereToStore)
{
	if (std::dynamic_pointer_cast<Operation>(node->GetRoot()) != nullptr)
	{
		TranslateExpressionNode(node->GetRoot());
		m_constructor->popFromStack(std::stol(whereToStore->GetAttribute(L"PositionOnStack").data()));
	}
	else
	{
		m_constructor->copyAtom(std::stol(node->GetRoot()->GetAttribute(L"PositionOnStack").data()), std::stol(whereToStore->GetAttribute(L"PositionOnStack").data()));
	}
}

void CodeGeneratorStage::TranslateExpressionNode(std::shared_ptr<AbstractTreeNode> node)
{
	auto operation = std::dynamic_pointer_cast<Operation>(node);

	auto leftLiteral = std::dynamic_pointer_cast<Literal>(operation->GetLeft());
	auto rightLiteral = std::dynamic_pointer_cast<Literal>(operation->GetRight());
	
	auto leftIdentificator = std::dynamic_pointer_cast<Identificator>(operation->GetLeft());
	auto rightIdentificator = std::dynamic_pointer_cast<Identificator>(operation->GetRight());

	auto leftOperation = std::dynamic_pointer_cast<Operation>(operation->GetLeft());
	auto rightOperation = std::dynamic_pointer_cast<Operation>(operation->GetRight());

	m_constructor->pushToStack(static_cast<int>(operation->GetOperationName()));

	if (leftLiteral != nullptr)
		m_constructor->pushToStack(m_constructor->GetLiteralPos(leftLiteral->GetData()));
	else if (leftIdentificator != nullptr)
		m_constructor->pushToStack(std::stol(leftIdentificator->GetAttribute(L"PositionOnStack").data()));
	else if(leftOperation != nullptr)
		TranslateExpressionNode(operation->GetLeft());

	if (rightLiteral != nullptr)
		m_constructor->pushToStack(m_constructor->GetLiteralPos(rightLiteral->GetData()));
	else if (rightIdentificator != nullptr)
		m_constructor->pushToStack(std::stol(rightIdentificator->GetAttribute(L"PositionOnStack").data()));
	else if (rightOperation != nullptr)
		TranslateExpressionNode(operation->GetRight());

	m_constructor->doOperationPush(m_constructor->GetCurrentStackOffset() - 1, m_constructor->GetCurrentStackOffset(), true);
}
