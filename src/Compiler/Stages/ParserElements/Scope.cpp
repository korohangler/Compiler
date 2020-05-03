#include "stdafx.h"
#include "Scope.h"
#include "CompilerParts/ParserHelper.h"

void Scope::Compute(const Token& token)
{
	if (m_counter == 0)
	{
		if (token.Type == L"CommonSeparator") return;

		ASSERT2(token.Type == L"Bracket" && token.Value == L"{",
			std::wstring(L"token type mismatch! Expected CurlyOpeningBracket, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	else if(m_counter == 1)
	{
		if (token.Type == L"Bracket" && token.Value == L"}" && (m_childs.empty() || m_childs.back()->IsComplete()))
		{
			m_needRecompute = false;
			m_isCompleted = true;
		}
		else
		{
			if (m_childs.empty() || m_childs.back()->IsComplete())
			{
				m_childs.emplace_back(ParserHelper::CreateNewNodeFromToken(token));
				m_childs.back()->parent = this;
			}

			m_childs.back()->Compute(token);

			m_needRecompute = m_childs.back()->NeedRecompute();
		}
	}
}
