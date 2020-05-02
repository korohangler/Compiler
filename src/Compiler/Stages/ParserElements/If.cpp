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
		ASSERT2(token.Type == L"If", std::wstring(L"token type mismatch! Expected If, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	// (
	else if(m_counter == 1)
	{
		ASSERT2(token.Type == L"RoundOpeningBracket", std::wstring(L"token type mismatch! Expected RoundOpeningBracket, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	// expression
	else if(m_counter == 2)
	{
		m_childs.emplace_back(std::make_shared<Expression>());

		m_childs.back()->Compute(token);

		m_counter++;
	}
	else if(m_counter == 3)
	{
		m_childs.back()->Compute(token);
		
		if (m_childs.back()->IsComplete()) m_counter++;
	}
	// )
	else if(m_counter == 4)
	{
		ASSERT2(token.Type == L"RoundCloseBracket", std::wstring(L"token type mismatch! Expected RoundCloseBracket, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	// {body}
	else if(m_counter == 5)
	{
		m_childs.emplace_back(std::make_shared<Scope>());

		m_childs.back()->Compute(token);
		m_counter++;
	}
	else if(m_counter == 6)
	{
		m_childs.back()->Compute(token);

		if (m_childs.back()->IsComplete()) m_counter++;
	}
	// else
	else if(m_counter == 7)
	{
		if (token.Type == L"Else")
		{
			m_counter++;
		}
		else
		{
			m_needRecompute = true;
			m_isComplete = true;
		}
	}
	else if(m_counter == 8)
	{
		m_counter = (token.Type == L"If") ? 0 : 5;
		
		m_needRecompute = true;
	}
}

bool If::NeedRecompute() const
{
	return m_needRecompute;
}
