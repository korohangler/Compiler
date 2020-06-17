#include "stdafx.h"
#include "Expression.h"
#include "CompilerParts/IdentificatorTable.h"
#include "Identificator.h"
#include "Literal.h"
#include "MathOperations/Operation.h"
#include "ExpressionStatement.h"

void Expression::Compute(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;
	
	if (token.Type == L"Identificator" 
		|| token.Type == L"BinaryOperator" || token.Type == L"UnaryOperator"
		|| token.Type == L"Bracket" && (token.Value == L"(" || token.Value == L")")
		|| token.Type == L"StringLiteral" || token.Type == L"DoubleLiteral" || token.Type == L"OctLiteral" || token.Type == L"HexLiteral")
	{
		m_tokens.push_back(token);
	}
	else
	{
		Utils::ASSERT2(!m_tokens.empty(), std::wstring(L"Expected expression at line: ") + std::to_wstring(token.Line));
		
		ConvertTokensToExpressionNodes();

		Childs.emplace_back(ParseExpression(m_convertedTokens.begin(), m_convertedTokens.end()));
		m_needRecompute = true;
		m_isComplete = true;
	}
}

void Expression::ConvertTokensToExpressionNodes()
{
	bool functionCallCreation = false;
	for (size_t i = 0; i < m_tokens.size(); i++)
	{
		if (functionCallCreation)
		{
			m_convertedTokens.back().node->Compute(m_tokens[i]);
			functionCallCreation = m_convertedTokens.back().node->IsComplete();
		} 
		else if (m_tokens[i].Type == L"BinaryOperator"
			|| m_tokens[i].Type == L"UnaryOperator"
			|| m_tokens[i].Type == L"DoubleLiteral"
			|| m_tokens[i].Type == L"OctLiteral"
			|| m_tokens[i].Type == L"HexLiteral"
			|| m_tokens[i].Type == L"StringLiteral")
		{
			m_convertedTokens.emplace_back(CreateNode(m_tokens[i]));
		}
		else if(m_tokens[i].Type == L"Identificator")
		{
			if (i >= m_tokens.size() - 2 || m_tokens[i + 1].Type != L"Bracket")
			{
				m_convertedTokens.emplace_back(CreateNode(m_tokens[i]));
			}
			else
			{
				ExpressionNode expressionStatement;
				expressionStatement.node = std::make_shared<ExpressionStatement>();
				expressionStatement.Type = L"ExpressionStatement";
				m_convertedTokens.emplace_back(expressionStatement);
				functionCallCreation = true;
			}
		}
	}
}

std::shared_ptr<AbstractTreeNode> Expression::ParseExpression(std::vector<ExpressionNode>::const_iterator begin, std::vector<ExpressionNode>::const_iterator end)
{
	auto last = end - 1;
	while(begin->Value == L"(" && last->Value == L")" && begin != last)
	{
		++begin;
		--end;
	}
	
	const auto root = GetMinPriorityToken(begin, end);

	const size_t distance = std::distance(begin, end);
	
	if (distance > 2)
	{
		const std::shared_ptr<Operation> operation = std::dynamic_pointer_cast<Operation>(root->node);

		operation->SetLeft(ParseExpression(begin, root));

		if (operation->GetOperationType() == OperationType::Binary)
		{
			operation->SetRight(ParseExpression(root + 1, end));
		}
	}
	else if (distance == 1)
	{
		return begin->node;
	}
	
	return root->node;
}

std::vector<Expression::ExpressionNode>::const_iterator
Expression::GetMinPriorityToken(std::vector<ExpressionNode>::const_iterator begin, std::vector<ExpressionNode>::const_iterator end)
{
	size_t openedBrackets = 0;
	size_t closedBrackets = 0;

	auto result = end;

	size_t currPriority = -1;
	
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
				const size_t newPriority = GetOperationPriority(*it);

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

size_t Expression::GetOperationPriority(const ExpressionNode& token)
{
	size_t result = -1;

	if(token.Value == L"++"
		|| token.Value == L"--")
	{
		result = 18;
	}
	else if (token.Type == L"UnaryOperator")
	{
		result = 17;
	}
	else if(token.Value == L"*"
		|| token.Value == L"/")
	{
		result = 15;
	}
	else if(token.Value == L"+"
		|| token.Value == L"-")
	{
		result = 14;
	}
	else if(token.Value == L">"
		|| token.Value == L"<"
		|| token.Value == L">="
		|| token.Value == L"<=")
	{
		result = 12;
	}
	else if(token.Value == L"=="
		|| token.Value == L"!="
		|| token.Value == L"!=="
		|| token.Value == L"===")
	{
		result = 11;
	}
	
	return result;
}

Expression::ExpressionNode Expression::CreateNode(const Token& token)
{
	ExpressionNode result;
	result.Type = token.Type;
	result.Value = token.Value;

	if (token.Type == L"Identificator")
	{
		result.node = std::make_shared<Identificator>(token.Value);
	}
	else if(token.Type == L"HexLiteral"
		|| token.Type == L"OctLiteral"
		|| token.Type == L"StringLiteral"
		|| token.Type == L"DoubleLiteral")
	{
		result.node = std::make_shared<Literal>(token);
	}
	else
	{
		result.node = std::make_shared<Operation>(token);
	}
	
	return result;
}
