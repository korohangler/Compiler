#include "stdafx.h"
#include "If.h"
#include "Expression.h"
#include "Scope.h"

void If::Compute(const Token& token)
{
	switch(m_state)
	{
	case States::IfKeyword:
		HandleIfKeyword(token);
		break;
	case States::OpeningBracket:
		HandleOpeningBracket(token);
		break;
	case States::ExpressionComputation:
		HandleExpressionComputation(token);
		break;
	case States::ScopeCreation:
		HandleScopeCreation(token);
		break;
	case States::ScopeComputation:
		HandleScopeComputation(token);
		break;
	case States::ElseKeyword:
		HandleElseKeyword(token);
		break;
	case States::ElseIfPossibility:
		HandleElseIfPossibility(token);
		break;
	default:
		Utils::ASSERT(std::string("Unknown state! At: ") + __FILE__);
	}
}

void If::HandleIfKeyword(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	Utils::ASSERT2(token.Type == L"Keyword" && token.Value == L"if",
		std::wstring(L"token type mismatch! Expected If, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_state = States::OpeningBracket;
}

void If::HandleOpeningBracket(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	Utils::ASSERT2(token.Type == L"Bracket" && token.Value == L"(",
		std::wstring(L"token type mismatch! Expected RoundOpeningBracket, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	Childs.emplace_back(std::make_shared<Expression>());
	
	m_needRecompute = true;

	m_state = States::ExpressionComputation;
}

void If::HandleExpressionComputation(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	Childs.back()->Compute(token);

	if (Childs.back()->IsComplete()) m_state = States::ScopeCreation;

	m_needRecompute = Childs.back()->NeedRecompute();
}

void If::HandleScopeCreation(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	Childs.emplace_back(std::make_shared<Scope>());

	m_needRecompute = true;

	m_state = States::ScopeComputation;
}

void If::HandleScopeComputation(const Token& token)
{
	Childs.back()->Compute(token);

	if (Childs.back()->IsComplete()) m_state = States::ElseKeyword;

	m_needRecompute = Childs.back()->NeedRecompute();
}

void If::HandleElseKeyword(const Token& token)
{
	if (token.Type == L"Keyword" && token.Value == L"else")
	{
		m_state = States::ElseIfPossibility;
	}
	else
	{
		m_needRecompute = true;
		m_isComplete = true;
	}
}

void If::HandleElseIfPossibility(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	m_state = (token.Type == L"Keyword" && token.Value == L"if") ? States::IfKeyword : States::ScopeCreation;

	m_needRecompute = true;
}
