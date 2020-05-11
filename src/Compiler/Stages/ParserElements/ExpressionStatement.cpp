#include "stdafx.h"
#include "ExpressionStatement.h"
#include "Expression.h"

void ExpressionStatement::Compute(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	switch(m_state)
	{
	case States::AwaitIdentificator:
		HandleAwaitIdentificatorState(token);
		break;
	case States::TypeChoise:
		HandleTypeChoiseState(token);
		break;
	case States::ExpressionComputation:
		HandleExpressionComputingState(token);
		break;
	case States::SemicolonAwait:
		HandleSemicolonAwaitState(token);
		break;
	case States::AwaitCloseBracket:
		HandleClosingBracketAwaitState(token);
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

	Childs.emplace_back(std::make_shared<Identificator>(token.Value));

	m_state = States::TypeChoise;
}

void ExpressionStatement::HandleTypeChoiseState(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;
	
	if (token.Type == L"Bracket" && token.Value == L"(")
	{
		m_type = StatementType::CallExpression;

		m_state = States::AwaitCloseBracket;
	}
	else if (token.Type == L"Assignment")
	{
		m_type = StatementType::AssignmentExpression;

		// Simple expression
		Childs.push_back(std::make_shared<Expression>());

		m_state = States::ExpressionComputation;
	}

	if (m_type == StatementType::AssignmentExpression)
		m_serializeData = L"AssignmentExpression: ";
	else if (m_type == StatementType::CallExpression)
		m_serializeData = L"CallExpression: ";

	m_serializeData += GetIdentificator()->GetVariableName();
}

void ExpressionStatement::HandleExpressionComputingState(const Token& token)
{
	Childs.back()->Compute(token);

	if (Childs.back()->IsComplete()) m_state = States::SemicolonAwait;

	m_needRecompute = Childs.back()->NeedRecompute();
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

	m_state = States::SemicolonAwait;
}
