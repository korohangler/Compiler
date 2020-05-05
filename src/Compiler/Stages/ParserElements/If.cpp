#include "stdafx.h"
#include "If.h"
#include "Expression.h"
#include "Scope.h"

void If::Compute(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	// if
	if(m_counter == 0)
	{
		Utils::ASSERT2(token.Type == L"Keyword" && token.Value == L"if", 
		               std::wstring(L"token type mismatch! Expected If, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	// (
	else if(m_counter == 1)
	{
		Utils::ASSERT2(token.Type == L"Bracket" && token.Value == L"(",
		               std::wstring(L"token type mismatch! Expected RoundOpeningBracket, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_childs.emplace_back(std::make_shared<Expression>());
		
		m_childs.back()->Compute(token);

		m_counter++;
	}
	else if(m_counter == 2)
	{
		m_childs.back()->Compute(token);
		
		if (m_childs.back()->IsComplete()) m_counter++;

		m_needRecompute = m_childs.back()->NeedRecompute();
	}
	// {body}
	else if(m_counter == 3)
	{
		m_childs.emplace_back(std::make_shared<Scope>());

		m_childs.back()->Compute(token);

		m_needRecompute = m_childs.back()->NeedRecompute();
		
		m_counter++;
	}
	else if(m_counter == 4)
	{
		m_childs.back()->Compute(token);

		if (m_childs.back()->IsComplete()) m_counter++;

		m_needRecompute = m_childs.back()->NeedRecompute();
	}
	// else
	else if(m_counter == 5)
	{
		if (token.Type == L"Keyword" && token.Value == L"else")
		{
			m_counter++;
		}
		else
		{
			m_needRecompute = true;
			m_isComplete = true;
		}
	}
	else if(m_counter == 6)
	{
		m_counter = (token.Type == L"Keyword" && token.Value == L"if") ? 0 : 5;
		
		m_needRecompute = true;
	}
}
