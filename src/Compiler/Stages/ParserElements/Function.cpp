#include "stdafx.h"
#include "Function.h"

#include "Identificator.h"
#include "Scope.h"

void Function::Compute(const Token& token)
{
	switch(m_state)
	{
	case States::FunctionKeyword:
		HandleFunctionKeyword(token);
		break;
	case States::Identificator:
		HandleIdentificator(token);
		break;
	case States::OpeningBracket:
		HandleOpeningBracket(token);
		break;
	case States::ArgumentOrBracket:
		HandleArgumentOrBracket(token);
		break;
	case States::CommaOrBracket:
		HandleCommaOrBracket(token);
		break;
	case States::ScopeCreation:
		HandleScopeCreation(token);
		break;
	case States::ScopeComputation:
		HandleScopeComputation(token);
		break;
	default:
		Utils::ASSERT(std::string("Unknown state! At: ") + __FILE__);
	}
}

void Function::HandleFunctionKeyword(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;
	Utils::ASSERT2(token.Type == L"Keyword" && token.Value == L"function",
		std::wstring(L"Token type mismatch! Expected: Keyword. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_state = States::Identificator;
}

void Function::HandleIdentificator(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	Utils::ASSERT2(token.Type == L"Identificator",
		std::wstring(L"Token type mismatch! Expected: Identificator. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_serializeData = m_functionName = token.Value;

	m_state = States::OpeningBracket;
}

void Function::HandleOpeningBracket(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	Utils::ASSERT2(token.Type == L"Bracket" && token.Value == L"(",
		std::wstring(L"Token type mismatch! Expected: Bracket. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_state = States::ArgumentOrBracket;
}

void Function::HandleArgumentOrBracket(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	if (token.Type == L"Bracket" && token.Value == L")")
	{
		m_state = States::ScopeCreation;
		return;
	}

	Utils::ASSERT2(token.Type == L"Identificator",
		std::wstring(L"Token type mismatch! Expected: Identificator. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	Childs.push_back(std::make_shared<Identificator>(token.Value));

	m_argumentsCount++;

	m_state = States::CommaOrBracket;
}

void Function::HandleCommaOrBracket(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	if (token.Type == L"Comma")
	{
		m_state = States::ArgumentOrBracket;
	}
	else if (token.Type == L"Bracket" && token.Value == L")")
	{
		m_state = States::ScopeCreation;
	}
	else
	{
		Utils::ASSERT(std::wstring(L"Token type mismatch! Expected Comma or Bracket but got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));
	}
}

void Function::HandleScopeCreation(const Token& token)
{
	Childs.emplace_back(std::make_shared<Scope>());

	m_state = States::ScopeComputation;

	m_needRecompute = true;
}

void Function::HandleScopeComputation(const Token& token)
{
	Childs.back()->Compute(token);

	m_needRecompute = Childs.back()->NeedRecompute();

	m_isComplete = Childs.back()->IsComplete();
}
