#include "stdafx.h"
#include "ParserHelper.h"
#include "Stages/ParserElements/Scope.h"
#include "Stages/ParserElements/Let.h"
#include "Stages/ParserElements/If.h"
#include "Stages/ParserElements/ExpressionStatement.h"
#include "Stages/ParserElements/Function.h"
#include "Stages/ParserElements/While.h"

std::shared_ptr<AbstractTreeNode> ParserHelper::CreateNewNodeFromToken(const Token& token)
{
	std::shared_ptr<AbstractTreeNode> result;

	if (token.Type == L"Identificator")
		result = std::make_shared<ExpressionStatement>();
	else if (token.Type == L"Bracket")
		result = std::make_shared<Scope>();
	else if (token.Type == L"Keyword" && (token.Value == L"let" || token.Value == L"var"))
		result = std::make_shared<Let>();
	else if (token.Type == L"Keyword" && token.Value == L"if")
		result = std::make_shared<If>();
	else if (token.Type == L"Keyword" && token.Value == L"function")
		result = std::make_shared<Function>();
	else if (token.Type == L"Keyword" && token.Value == L"while")
		result = std::make_shared<While>();
	
	return result;
}
