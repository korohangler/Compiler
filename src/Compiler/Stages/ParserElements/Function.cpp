#include "stdafx.h"
#include "Function.h"

#include "Identificator.h"
#include "Scope.h"

void Function::Compute(const Token& token)
{
	switch(m_counter)
	{
	case 0:
		HandleFunctionKeyword(token);
		break;
	case 1:
		HandleIdentificator(token);
		break;
	case 2:
		HandleOpeningBracket(token);
		break;
	case 3:
		HandleArgumentOrBracket(token);
		break;
	case 4:
		HandleCommaOrBracket(token);
		break;
	case 5:
		HandleScopeCreation(token);
		break;
	case 6:
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

	m_counter++;
}

void Function::HandleIdentificator(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	Utils::ASSERT2(token.Type == L"Identificator",
		std::wstring(L"Token type mismatch! Expected: Identificator. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_serializeData = m_functionName = token.Value;

	m_counter++;
}

void Function::HandleOpeningBracket(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	Utils::ASSERT2(token.Type == L"Bracket" && token.Value == L"(",
		std::wstring(L"Token type mismatch! Expected: Bracket. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_counter++;
}

void Function::HandleArgumentOrBracket(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	if (token.Type == L"Bracket" && token.Value == L")")
	{
		m_counter = 5;
		return;
	}

	Utils::ASSERT2(token.Type == L"Identificator",
		std::wstring(L"Token type mismatch! Expected: Identificator. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_childs.push_back(std::make_shared<Identificator>(token.Value));

	m_counter++;
}

void Function::HandleCommaOrBracket(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	if (token.Type == L"Comma")
	{
		m_counter = 3;
	}
	else if (token.Type == L"Bracket" && token.Value == L")")
	{
		m_counter++;
	}
	else
	{
		Utils::ASSERT(std::wstring(L"Token type mismatch! Expected Comma or Bracket but got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));
	}
}

void Function::HandleScopeCreation(const Token& token)
{
	m_childs.emplace_back(std::make_shared<Scope>());

	m_counter++;

	m_needRecompute = true;
}

void Function::HandleScopeComputation(const Token& token)
{
	m_childs.back()->Compute(token);

	m_needRecompute = m_childs.back()->NeedRecompute();

	m_isComplete = m_childs.back()->IsComplete();
}
