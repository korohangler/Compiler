#include "stdafx.h"
#include "Literal.h"

Literal::Literal(const Token& token)
{
	if (token.Type == L"StringLiteral")
	{
		m_literalType = LiteralType::String;
		m_data = token.Value;
	}
	else if(token.Type == L"HexLiteral"
		|| token.Type == L"OctLiteral")
	{
		int dummy;

		std::wstringstream ss;
		ss << (token.Type == L"HexLiteral" ? std::hex : std::oct) << token.Value;
		ss >> dummy;

		m_data = std::to_wstring(dummy);

		m_literalType = LiteralType::Number;
	}
	else if(token.Type == L"DoubleLiteral")
	{
		m_literalType = LiteralType::Number;
		m_data = token.Value;
	}
	else
	{
		ASSERT(std::wstring(L"Unexpected token type! Expected: Literal. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));
	}

	m_serializeData = std::wstring(L"Value: ") + m_data + std::wstring(L". Type: ") + (m_literalType == LiteralType::Number ? L"Number" : L"String");
}
