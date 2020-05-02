#include "stdafx.h"
#include "For.h"

bool For::IsComplete() const
{
	return false;
}

void For::Compute(const Token& token)
{
	// for
	if (m_counter == 0)
	{
		ASSERT2(token.Type == L"For", std::wstring(L"token type mismatch! Expected For, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	// (
	else if(m_counter == 1)
	{
		if (token.Type == L"CommonSeparator") return;
		
		ASSERT2(token.Type == L"RoundOpeningBracket", std::wstring(L"token type mismatch! Expected RoundOpeningBracket, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	else if (m_counter == 2)
	{
		if (token.Type == L"CommonSeparator") return;

		ASSERT2(token.Type == L"RoundOpeningBracket", std::wstring(L"token type mismatch! Expected RoundOpeningBracket, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
}
