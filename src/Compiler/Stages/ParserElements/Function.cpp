#include "stdafx.h"
#include "Function.h"

#include "Identificator.h"
#include "Scope.h"

void Function::Compute(const Token& token)
{
	if (m_counter == 0)
	{
		if (token.Type == L"CommonSeparator") return;
		ASSERT2(token.Type == L"Keyword" && token.Value == L"function",
			std::wstring(L"Token type mismatch! Expected: Keyword. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	else if(m_counter == 1)
	{
		if (token.Type == L"CommonSeparator") return;

		ASSERT2(token.Type == L"Identificator",
			std::wstring(L"Token type mismatch! Expected: Identificator. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_functionName = token.Value;

		m_serializeData = m_functionName;
		
		m_counter++;
	}
	else if (m_counter == 2)
	{
		if (token.Type == L"CommonSeparator") return;

		ASSERT2(token.Type == L"Bracket" && token.Value == L"(",
			std::wstring(L"Token type mismatch! Expected: Bracket. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	else if(m_counter == 3)
	{
		if (token.Type == L"CommonSeparator") return;

		if (token.Type == L"Bracket" && token.Value == L")")
		{
			m_counter = 5;
			return;
		}
		
		ASSERT2(token.Type == L"Identificator",
			std::wstring(L"Token type mismatch! Expected: Identificator. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_childs.push_back(std::make_shared<Identificator>(token.Value));
		
		m_counter++;
	}
	else if(m_counter == 4)
	{
		if (token.Type == L"CommonSeparator") return;
		
		if (token.Type == L"Comma")
		{
			m_counter = 3;
		}
		else if(token.Type == L"Bracket" && token.Value == L")")
		{
			m_counter++;
		}
		else
		{
			ASSERT(std::wstring(L"Token type mismatch! Expected Comma or Bracket but got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));
		}
	}
	else if(m_counter == 5)
	{
		m_childs.emplace_back(std::make_shared<Scope>());

		m_counter++;

		m_needRecompute = true;
	}
	else if(m_counter == 6)
	{
		m_childs.back()->Compute(token);

		m_needRecompute = m_childs.back()->NeedRecompute();

		m_isComplete = m_childs.back()->IsComplete();
	}
}
