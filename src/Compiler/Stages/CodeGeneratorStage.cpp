#include "stdafx.h"
#include "CodeGeneratorStage.h"


#include "ParserElements/ExpressionStatement.h"
#include "ParserElements/Function.h"
#include "ParserElements/If.h"
#include "ParserElements/Let.h"
#include "ParserElements/Scope.h"
#include "ParserElements/While.h"

void CodeGeneratorStage::Notify(std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>> data)
{
	m_table = data.second;

	
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
}

void CodeGeneratorStage::TranslateExpressionStatement(std::shared_ptr<ExpressionStatement> node)
{
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
