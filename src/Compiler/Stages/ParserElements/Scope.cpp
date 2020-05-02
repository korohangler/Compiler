#include "stdafx.h"
#include "Scope.h"
#include "CompilerParts/ParserHelper.h"

void Scope::Compute(const Token& token)
{
	if (m_isFirstToken)
	{
		ASSERT2(token.Type == L"CurlyOpeningBracket", std::wstring(L"token type mismatch! Expected CurlyOpeningBracket, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));
		m_isFirstToken = false;
		return;
	}
	
	m_isCompleted = token.Type == L"CurlyCloseBracket" && (m_childs.empty() || m_childs.back()->IsComplete());

	if (!m_isCompleted)
	{
		if (m_childs.empty() || m_childs.back()->IsComplete())
		{
			m_childs.emplace_back(ParserHelper::CreateNewNodeFromToken(token));
			m_childs.back()->parent = this;
		}
		
		m_childs.back()->Compute(token);
	}
}
