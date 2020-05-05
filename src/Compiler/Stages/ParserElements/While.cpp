#include "stdafx.h"
#include "While.h"

#include "Expression.h"
#include "Scope.h"

void While::Compute(const Token& token)
{
	if (m_counter == 0)
	{
		if (token.Type == L"CommonSeparator") return;
		ASSERT2(token.Type == L"Keyword" && token.Value == L"while",
			std::wstring(L"Token type mismatch! Expected: Keyword. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	else if (m_counter == 1)
	{
		m_childs.emplace_back(std::make_shared<Expression>());
		m_childs.back()->parent = this;

		m_needRecompute = true;

		m_counter++;
	}
	else if (m_counter == 2)
	{
		m_childs.back()->Compute(token);

		m_needRecompute = m_childs.back()->NeedRecompute();

		if (m_childs.back()->IsComplete()) m_counter++;
	}
	else if (m_counter == 3)
	{
		m_childs.emplace_back(std::make_shared<Scope>());

		m_needRecompute = true;
		
		m_counter++;
	}
	else if (m_counter == 4)
	{
		m_childs.back()->Compute(token);

		m_needRecompute = m_childs.back()->NeedRecompute();

		m_isCompleted = m_childs.back()->IsComplete();
	}
}
