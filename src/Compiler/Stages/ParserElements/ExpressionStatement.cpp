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
	case States::Argument:
		HandleArgument(token);
		break;
	case States::Comma:
		HandleComma(token);
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

		m_state = States::Argument;
	}
	else if (token.Type == L"BinaryOperator" && token.Value == L"=")
	{
		m_type = StatementType::AssignmentExpression;

		// Simple expression
		Childs.push_back(std::make_shared<Expression>());

		m_state = States::ExpressionComputation;
	}
	else
	{
		Utils::ASSERT(
			std::wstring(L"Token type mismatch! Expected Identificator or Bracket. But got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));
	}

	if (m_type == StatementType::AssignmentExpression)
		m_serializeData = L"AssignmentExpression: ";
	else if (m_type == StatementType::CallExpression)
		m_serializeData = L"CallExpression: ";

	m_serializeData += GetIdentificator()->GetName();
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

void ExpressionStatement::HandleArgument(const Token& token)
{
	if (token.Type == L"Bracket" && token.Value == L")")
	{
		m_state = States::SemicolonAwait;
	}
	else
	{
		Utils::ASSERT2(token.Type == L"Identificator",
			std::wstring(L"Token type mismatch! Expected Identificator. But got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_arguments.emplace_back(std::make_shared<Identificator>(token.Value));

		m_state = States::Comma;
	}
}

void ExpressionStatement::HandleComma(const Token& token)
{
	if (token.Type == L"Comma")
	{
		m_state = States::Argument;
	}
	else if (token.Type == L"Bracket" && token.Value == L")")
	{
		m_state = States::SemicolonAwait;
	}
	else
	{
		Utils::ASSERT(
			std::wstring(L"Token type mismatch! Expected Comma or Bracket. But got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));
	}
}
