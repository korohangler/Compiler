#include "stdafx.h"
#include "While.h"

#include "Expression.h"
#include "Scope.h"

void While::Compute(const Token& token)
{
	switch(m_state)
	{
	case States::KeywordWhile:
		HandleKeywordWhile(token);
		break;
	case States::ExpressionCreation:
		HandleExpressionCreation(token);
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
	default:
		Utils::ASSERT(std::string("Unknown state! At line: ") + std::to_string(__LINE__) + std::string(". In file: ") + __FILE__);
	}
}

void While::HandleKeywordWhile(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;
	Utils::ASSERT2(token.Type == L"Keyword" && token.Value == L"while",
		std::wstring(L"Token type mismatch! Expected: Keyword. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_state = States::ExpressionCreation;
}

void While::HandleExpressionCreation(const Token& token)
{
	Childs.emplace_back(std::make_shared<Expression>());
	Childs.back()->Parent = this;

	m_needRecompute = true;

	m_state = States::ExpressionComputation;
}

void While::HandleExpressionComputation(const Token& token)
{
	Childs.back()->Compute(token);

	m_needRecompute = Childs.back()->NeedRecompute();

	if (Childs.back()->IsComplete()) m_state = States::ScopeCreation;
}

void While::HandleScopeCreation(const Token& token)
{
	Childs.emplace_back(std::make_shared<Scope>());

	m_needRecompute = true;

	m_state = States::ScopeComputation;
}

void While::HandleScopeComputation(const Token& token)
{
	Childs.back()->Compute(token);

	m_needRecompute = Childs.back()->NeedRecompute();

	m_isComplete = Childs.back()->IsComplete();
}
