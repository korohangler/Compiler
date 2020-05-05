#include "stdafx.h"
#include "ExpressionStatement.h"
#include "Expression.h"

void ExpressionStatement::Compute(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	switch(m_counter)
	{
	case 0:
		HandleAwaitIdentificatorState(token);
		break;
	case 1:
		HandleTypeChoiseState(token);
		break;
	case 2:
		HandleExpressionComputingState(token);
		break;
	case 3:
		HandleSemicolonAwaitState(token);
		break;
	case 4:
		HandleClosingBracketAwaitState(token);
		break;
	case 5:
		HandleSemicolonAwaitState(token);
		break;
	default:
		Utils::ASSERT(std::string("Unknown state! At: ") + __FILE__);
	}
}

void ExpressionStatement::HandleAwaitIdentificatorState(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;
	
	Utils::ASSERT2(token.Type == L"Identificator",
		std::wstring(L"Token type mismatch! Expected: Identificator. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_identificator = token.Value;

	m_counter++;
}

void ExpressionStatement::HandleTypeChoiseState(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;
	
	if (token.Type == L"Bracket" && token.Value == L"(")
	{
		m_type = StatementType::CallExpression;

		m_counter = 4;
	}
	else if (token.Type == L"Assignment")
	{
		m_type = StatementType::AssignmentExpression;

		// Simple expression
		m_childs.push_back(std::make_shared<Expression>());

		m_counter++;
	}

	if (m_type == StatementType::AssignmentExpression)
		m_serializeData = L"AssignmentExpression: ";
	else if (m_type == StatementType::CallExpression)
		m_serializeData = L"CallExpression: ";

	m_serializeData += m_identificator;
}

void ExpressionStatement::HandleExpressionComputingState(const Token& token)
{
	m_childs.back()->Compute(token);

	if (m_childs.back()->IsComplete()) m_counter++;

	m_needRecompute = m_childs.back()->NeedRecompute();
}

void ExpressionStatement::HandleSemicolonAwaitState(const Token& token)
{
	Utils::ASSERT2(token.Type == L"Semicolon",
		std::wstring(L"Token type mismatch! Expected: Semicolon. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_isComplete = true;
	m_needRecompute = false;
}

void ExpressionStatement::HandleClosingBracketAwaitState(const Token& token)
{
	Utils::ASSERT2(token.Type == L"Bracket" && token.Value == L")",
		std::wstring(L"Token type mismatch! Expected Bracket. But got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_counter++;
}
