#include "stdafx.h"
#include "CodeGeneratorStage.h"


#include "ParserElements/ExpressionStatement.h"
#include "ParserElements/Function.h"
#include "ParserElements/Identificator.h"
#include "ParserElements/If.h"
#include "ParserElements/Let.h"
#include "ParserElements/Literal.h"
#include "ParserElements/MathOperations/IOperation.h"
#include "ParserElements/Scope.h"
#include "ParserElements/While.h"
#include "ParserElements/Expression.h"

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
		for (auto child : scope->m_childs) TranslateNode(child);
	else if (function != nullptr)
		TranslateFunction(function);
}

void CodeGeneratorStage::TranslateLet(std::shared_ptr<Let> node)
{
	// TODO For now no initialization at all
	m_constructor->AddVariable(node->GetVariableName());
}

void CodeGeneratorStage::TranslateExpressionStatement(std::shared_ptr<ExpressionStatement> node)
{
	// TODO functions

	TranslateExpression(node->GetExpression(), node->GetIdentificatorName());
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

void CodeGeneratorStage::TranslateExpression(std::shared_ptr<Expression> node, std::wstring whereToStore)
{
	TranslateExpressionNode(node->GetRoot());
	m_constructor->fstp(whereToStore);
}

void CodeGeneratorStage::TranslateExpressionNode(std::shared_ptr<AbstractTreeNode> node)
{
	auto operation = std::dynamic_pointer_cast<IOperation>(node);

	auto leftLiteral = std::dynamic_pointer_cast<Literal>(operation->GetLeft());
	auto rightLiteral = std::dynamic_pointer_cast<Literal>(operation->GetRight());
	
	auto leftIdentificator = std::dynamic_pointer_cast<Identificator>(operation->GetLeft());
	auto rightIdentificator = std::dynamic_pointer_cast<Identificator>(operation->GetRight());

	auto leftOperation = std::dynamic_pointer_cast<IOperation>(operation->GetLeft());
	auto rightOperation = std::dynamic_pointer_cast<IOperation>(operation->GetRight());

	if (leftLiteral != nullptr)
		m_constructor->fld(m_constructor->GetConstant(leftLiteral->GetFloatData()));
	else if (leftIdentificator != nullptr)
		m_constructor->fld(leftIdentificator->GetVariableName());
	else if(leftOperation != nullptr)
		TranslateExpressionNode(operation->GetLeft());

	if (rightLiteral != nullptr)
		m_constructor->fld(m_constructor->GetConstant(rightLiteral->GetFloatData()));
	else if (rightIdentificator != nullptr)
		m_constructor->fld(rightIdentificator->GetVariableName());
	else if (rightOperation != nullptr)
		TranslateExpressionNode(operation->GetRight());

	m_constructor->faddp();
}
