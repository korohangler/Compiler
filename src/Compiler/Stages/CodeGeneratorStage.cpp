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
#include "ParserElements/MathOperations/Operation.h"

void CodeGeneratorStage::Notify(std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>> data)
{
	m_table = data.second;

	TranslateNode(data.first);

	NotifyListeners(m_constructor);
}

void CodeGeneratorStage::TranslateNode(const std::shared_ptr<AbstractTreeNode> node)
{
	const auto let = std::dynamic_pointer_cast<Let>(node);
	const auto expressionStatement = std::dynamic_pointer_cast<ExpressionStatement>(node);
	const auto ifElem = std::dynamic_pointer_cast<If>(node);
	const auto whileElem = std::dynamic_pointer_cast<While>(node);
	const auto scope = std::dynamic_pointer_cast<Scope>(node);
	const auto function = std::dynamic_pointer_cast<Function>(node);

	if (let != nullptr)
		TranslateLet(let);
	else if (expressionStatement != nullptr)
		TranslateExpressionStatement(expressionStatement);
	else if (ifElem != nullptr)
		TranslateIfElement(ifElem);
	else if (whileElem != nullptr)
		TranslateWhileElement(whileElem);
	else if (scope != nullptr)
		for (const auto child : scope->Childs) TranslateNode(child);
	else if (function != nullptr)
		TranslateFunction(function);
}

void CodeGeneratorStage::TranslateLet(const std::shared_ptr<Let> node)
{
	// TODO For now no initialization at all
	m_constructor->AddVariable(node->GetVariable());
}

void CodeGeneratorStage::TranslateExpressionStatement(const std::shared_ptr<ExpressionStatement> node)
{
	if (node->GetType() == ExpressionStatement::StatementType::CallExpression)
	{
		TranslateFunctionCall(node);
	}
	else
	{
		TranslateExpression(node->GetExpression(), node->GetIdentificator());
	}
}

void CodeGeneratorStage::TranslateIfElement(const std::shared_ptr<If> node)
{
	std::wstring newJumpMark = std::wstring(L"JumpMark") + std::to_wstring(m_jumpNameMarks);
	m_jumpNameMarks++;

	std::shared_ptr<Expression> expr = std::static_pointer_cast<Expression>(node->Childs[0]);

	// translate expression. Result on stack
	TranslateExpressionNode(expr->GetRoot());

	// add jump instruction if res == 1;
	m_constructor->jumpIf(newJumpMark);
	TranslateNode(node->Childs[1]);

	// add where to jump
	m_constructor->addJumpMark(newJumpMark);
}

void CodeGeneratorStage::TranslateWhileElement(const std::shared_ptr<While> node)
{
	std::wstring cycleBeginMark = std::wstring(L"JumpMark") + std::to_wstring(m_jumpNameMarks);
	m_jumpNameMarks++;

	m_constructor->addJumpMark(cycleBeginMark);

	std::wstring exprJumpMark = std::wstring(L"JumpMark") + std::to_wstring(m_jumpNameMarks);
	m_jumpNameMarks++;

	std::shared_ptr<Expression> expr = std::static_pointer_cast<Expression>(node->Childs[0]);

	// translate expression. Result on stack
	TranslateExpressionNode(expr->GetRoot());

	// add jump instruction if res == 1;
	m_constructor->jumpIf(exprJumpMark);
	TranslateNode(node->Childs[1]);

	m_constructor->jump(cycleBeginMark);

	// add where to jump
	m_constructor->addJumpMark(exprJumpMark);
}

void CodeGeneratorStage::TranslateFunction(const std::shared_ptr<Function> node)
{
	m_constructor->BeginFuction(node->GetAttribute(L"ASMName"));

	std::for_each(begin(node->Childs), end(node->Childs), [&](const auto& child) { TranslateNode(child); });

	m_constructor->EndFuction();
}

void CodeGeneratorStage::TranslateExpression(const std::shared_ptr<Expression> node, const std::shared_ptr<Identificator> whereToStore)
{
	if (std::dynamic_pointer_cast<Operation>(node->GetRoot()) != nullptr)
	{
		TranslateExpressionNode(node->GetRoot());
		m_constructor->popFromStack(std::stol(whereToStore->GetAttribute(L"PositionOnStack").data()));
	}
	else
	{
		if(std::dynamic_pointer_cast<Literal>(node->GetRoot()) != nullptr)
			m_constructor->AddLiteral(std::static_pointer_cast<Literal>(node->GetRoot()));
		m_constructor->copyAtom(std::stol(node->GetRoot()->GetAttribute(L"PositionOnStack").data()), std::stol(whereToStore->GetAttribute(L"PositionOnStack").data()));
	}
}

void CodeGeneratorStage::TranslateExpressionNode(const std::shared_ptr<AbstractTreeNode> node)
{
	const auto operation = std::dynamic_pointer_cast<Operation>(node);

	const auto leftLiteral = std::dynamic_pointer_cast<Literal>(operation->GetLeft());
	const auto rightLiteral = std::dynamic_pointer_cast<Literal>(operation->GetRight());
	
	const auto leftIdentificator = std::dynamic_pointer_cast<Identificator>(operation->GetLeft());
	const auto rightIdentificator = std::dynamic_pointer_cast<Identificator>(operation->GetRight());

	const auto leftOperation = std::dynamic_pointer_cast<Operation>(operation->GetLeft());
	const auto rightOperation = std::dynamic_pointer_cast<Operation>(operation->GetRight());

	m_constructor->pushValueToStack(static_cast<int>(operation->GetOperationName()));

	if (rightLiteral != nullptr)
	{
		m_constructor->AddLiteral(rightLiteral);
		m_constructor->pushToStack(std::stol(m_constructor->GetLiterPosition(rightLiteral->GetData()).data()));
	}
	else if (rightIdentificator != nullptr)
		m_constructor->pushToStack(std::stol(rightIdentificator->GetAttribute(L"PositionOnStack").data()));
	else if (rightOperation != nullptr)
		TranslateExpressionNode(operation->GetRight());

	if (leftLiteral != nullptr)
	{
		m_constructor->AddLiteral(leftLiteral);
		m_constructor->pushToStack(std::stol(m_constructor->GetLiterPosition(leftLiteral->GetData()).data()));
	}
	else if (leftIdentificator != nullptr)
		m_constructor->pushToStack(std::stol(leftIdentificator->GetAttribute(L"PositionOnStack").data()));
	else if(leftOperation != nullptr)
		TranslateExpressionNode(operation->GetLeft());

	m_constructor->doOperationPush(m_constructor->GetCurrentStackOffset() - 1, m_constructor->GetCurrentStackOffset(), true);
}

void CodeGeneratorStage::TranslateFunctionCall(const std::shared_ptr<ExpressionStatement> node)
{
	const auto& args = node->GetArguments();

	std::for_each(args.rbegin(), args.rend(), [&](const auto& elem) { m_constructor->pushToStack(std::stol(elem->GetAttribute(L"PositionOnStack").data())); });

	m_constructor->call(node->GetAttribute(L"ASMName"), -1, false, true);

	for (size_t i = 0; i < args.size(); i++) m_constructor->popFromStack();
}
