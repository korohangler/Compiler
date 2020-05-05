#include "stdafx.h"
#include "Let.h"
#include "Expression.h"

void Let::Compute(const Token& token)
{
	if (m_counter == 0)
	{
		ASSERT2(token.Type == L"Keyword" && (token.Value == L"var" || token.Value == L"let"), 
			std::wstring(L"token type mismatch! Expected Let, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	else if(m_counter == 1)
	{
		ASSERT2(token.Type == L"CommonSeparator", std::wstring(L"token type mismatch! Expected CommonSeparator, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	else if (m_counter == 2)
	{
		if (token.Type == L"CommonSeparator") return;
		
		ASSERT2(token.Type == L"Identificator", std::wstring(L"token type mismatch! Expected CommonSeparator, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_variableName = token.Value;
		
		m_counter++;
	}
	else if(m_counter == 3)
	{
		if (token.Type == L"CommonSeparator") return;

		if (token.Type == L"Semicolon")
		{
			m_isComplete = true;
		}
		else if(token.Type == L"Assignment")
		{
			m_childs.emplace_back(std::make_shared<Expression>());
			m_childs.back()->parent = this;
			
			m_counter++;
		}
	}
	else if(m_counter == 4)
	{
		if(m_childs.back()->IsComplete())
		{
			m_isComplete = true;
		}
		else
		{
			m_childs.back()->Compute(token);

			if(m_childs.back()->IsComplete())
			{
				m_counter++;
				m_needRecompute = true;
			}
		}
	}
	else if(m_counter == 5)
	{
		ASSERT2(token.Type == L"Semicolon",
			std::wstring(L"Expected a ; but got: ") + token.Value + std::wstring(L" at line: ") + std::to_wstring(token.Line));

		m_needRecompute = false;
		m_isComplete = true;
	}
}
