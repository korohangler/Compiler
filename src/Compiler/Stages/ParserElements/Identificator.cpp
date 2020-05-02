#include "stdafx.h"
#include "Identificator.h"

void Identificator::Compute(const Token& token)
{
	ASSERT2(token.Type == L"Identificator", std::wstring(L"token type mismatch! Expected Identificator, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));
	m_variableName = token.Value;
}
