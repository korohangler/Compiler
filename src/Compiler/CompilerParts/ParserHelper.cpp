#include "stdafx.h"
#include "ParserHelper.h"
#include "Stages/ParserElements/Scope.h"
#include "Stages/ParserElements/Identificator.h"
#include "Stages/ParserElements/Let.h"

std::shared_ptr<AbstractTreeNode> ParserHelper::CreateNewNodeFromToken(const Token& token)
{
	std::shared_ptr<AbstractTreeNode> result;

	if (token.Type == L"Identificator")
		result = std::make_shared<Identificator>();
	else if (token.Type == L"Bracket")
		result = std::make_shared<Scope>();
	else if (token.Type == L"Keyword" && (token.Value == L"let" || token.Value == L"var"))
		result = std::make_shared<Let>();
	
	return result;
}
