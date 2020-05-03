#include "stdafx.h"
#include "Expression.h"
#include "CompilerParts/IdentificatorTable.h"
#include "Identificator.h"
#include "Literal.h"
#include "Stages/ParserElements/MathOperations/DivideOperation.h"
#include "Stages/ParserElements/MathOperations/MinusOperation.h"
#include "Stages/ParserElements/MathOperations/MultiplyOperation.h"
#include "Stages/ParserElements/MathOperations/SummOperation.h"

void Expression::Compute(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;
	
	if (token.Type == L"Identificator")
	{
		ASSERT2(IdentificatorTable::GetInstance().IsVariableExist(GetScopeName(),token.Value), 
			std::wstring(L"Error! Use of undefined value: ") + token.Value + std::wstring(L" at line: ") + std::to_wstring(token.Line));
		m_tokens.push_back(token);
	}
	else if(token.Type == L"BinaryOperator" || token.Type == L"UnaryOperator"
		|| token.Type == L"Bracket" && (token.Value == L"(" || token.Value == L")")
		|| token.Type == L"StringLiteral" || token.Type == L"DoubleLiteral" || token.Type == L"OctLiteral" || token.Type == L"HexLiteral")
	{
		m_tokens.push_back(token);
	}
	else
	{
		ASSERT2(!m_tokens.empty(), std::wstring(L"Expected expression at line: ") + std::to_wstring(token.Line));
		
		m_childs.emplace_back(ParseExpression(m_tokens.begin(), m_tokens.end()));
		m_needRecompute = true;
		m_isComplete = true;
	}
}

std::shared_ptr<AbstractTreeNode> Expression::ParseExpression(std::vector<Token>::const_iterator begin, std::vector<Token>::const_iterator end)
{
	const auto rootToken = GetMinPriorityToken(begin, end);

	std::shared_ptr<AbstractTreeNode> expressionTreeRoot = CreateNode(rootToken == end ? *begin : *rootToken);

	const size_t distance = std::distance(begin, end);
	
	if (distance > 2)
	{
		const std::shared_ptr<IOperation> operation = std::dynamic_pointer_cast<IOperation>(expressionTreeRoot);

		operation->SetLeft(ParseExpression(begin, rootToken));

		if (operation->GetOperationType() == IOperation::OperationType::Binary)
		{
			operation->SetRight(ParseExpression(rootToken + 1, end));
		}
	}
	
	return expressionTreeRoot;
}

std::vector<Token>::const_iterator Expression::GetMinPriorityToken(std::vector<Token>::const_iterator begin, std::vector<Token>::const_iterator end)
{
	size_t openedBrackets = 0;
	size_t closedBrackets = 0;

	auto result = end;

	IOperation::OperationPriority currPriority = IOperation::OperationPriority::Highest;
	
	for (auto it = begin; it != end; ++it)
	{
		if (it->Type == L"Bracket")
		{
			if (it->Value == L"(")
				openedBrackets++;
			else
				closedBrackets++;
		}
		else if(it->Type == L"BinaryOperator")
		{
			if (openedBrackets == closedBrackets)
			{
				const IOperation::OperationPriority newPriority = GetOperationPriority(*it);

				if (newPriority <= currPriority)
				{
					currPriority = newPriority;

					result = it;
				}
			}
		}
	}
	
	return result;
}

IOperation::OperationPriority Expression::GetOperationPriority(const Token& token)
{
	IOperation::OperationPriority result = IOperation::OperationPriority::Unknown;
	
	if (token.Type == L"UnaryOperator")
	{
		result = IOperation::OperationPriority::Highest;
	}
	else if(token.Value == L"*"
		|| token.Value == L"/")
	{
		result = IOperation::OperationPriority::High;
	}
	else if(token.Value == L"+"
		|| token.Value == L"-")
	{
		result = IOperation::OperationPriority::Medium;
	}

	ASSERT2(result != IOperation::OperationPriority::Unknown,
		std::wstring(L"Cannot compute operator priority for token") + token.Value + std::wstring(L" at line: ") + std::to_wstring(token.Line));
	
	return result;
}

std::shared_ptr<AbstractTreeNode> Expression::CreateNode(const Token& token)
{
	std::shared_ptr<AbstractTreeNode> result;

	if (token.Type == L"Identificator")
	{
		result = std::make_shared<Identificator>(token.Value);
	}
	else if(token.Type == L"HexLiteral"
		|| token.Type == L"OctLiteral"
		|| token.Type == L"StringLiteral"
		|| token.Type == L"DoubleLiteral")
	{
		result = std::make_shared<Literal>(token);
	}
	else
	{
		if (token.Value == L"-")
			result = std::make_shared<MinusOperation>();
		else if (token.Value == L"+")
			result = std::make_shared<SummOperation>();
		else if (token.Value == L"*")
			result = std::make_shared<MultiplyOperation>();
	}
	
	return result;
}
