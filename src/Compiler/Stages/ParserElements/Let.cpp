#include "stdafx.h"
#include "Let.h"
#include "Expression.h"

void Let::Compute(const Token& token)
{
	switch(m_state)
	{
	case States::LetKeyword:
		HandleKeyWord(token);
		break;
	case States::CommonSeparator:
		HandleCommonSeparator(token);
		break;
	case States::Identificator:
		HandleIdentificator(token);
		break;
	case States::SemicolonOrAssign:
		HandleSemicolonOrAssign(token);
		break;
	case States::ExpressionComputation:
		HandleExpressionComputation(token);
		break;
	case States::Semicolon:
		HandleSemicolon(token);
		break;
	default:
		Utils::ASSERT(std::string("Unknown state! At line: ") + std::to_string(__LINE__) + std::string(". In file: ") + __FILE__);
	}
}

void Let::HandleKeyWord(const Token& token)
{
	Utils::ASSERT2(token.Type == L"Keyword" && (token.Value == L"var" || token.Value == L"let"),
		std::wstring(L"token type mismatch! Expected Let, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_state = States::CommonSeparator;
}

void Let::HandleCommonSeparator(const Token& token)
{
	Utils::ASSERT2(token.Type == L"CommonSeparator", std::wstring(L"token type mismatch! Expected CommonSeparator, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_state = States::Identificator;
}

void Let::HandleIdentificator(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	Utils::ASSERT2(token.Type == L"Identificator", std::wstring(L"token type mismatch! Expected CommonSeparator, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_variableName = token.Value;

	m_state = States::SemicolonOrAssign;
}

void Let::HandleSemicolonOrAssign(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	if (token.Type == L"Semicolon")
	{
		m_isComplete = true;
	}
	else if (token.Type == L"Assignment")
	{
		m_childs.emplace_back(std::make_shared<Expression>());
		m_childs.back()->parent = this;

		m_state = States::ExpressionComputation;
	}
}

void Let::HandleExpressionComputation(const Token& token)
{
	if (m_childs.back()->IsComplete())
	{
		m_isComplete = true;
	}
	else
	{
		m_childs.back()->Compute(token);

		if (m_childs.back()->IsComplete())
		{
			m_state = States::Semicolon;
			m_needRecompute = true;
		}
	}
}

void Let::HandleSemicolon(const Token& token)
{
	Utils::ASSERT2(token.Type == L"Semicolon",
		std::wstring(L"Expected a ; but got: ") + token.Value + std::wstring(L" at line: ") + std::to_wstring(token.Line));

	m_needRecompute = false;
	m_isComplete = true;
}
