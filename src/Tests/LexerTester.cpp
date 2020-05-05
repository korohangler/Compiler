#include "stdafx.h"
#include "LexerTester.h"

#include <iostream>

LexerTester::LexerTester(const std::vector<Token>& expectedTokens)
	: m_expectedTokens(expectedTokens)
	, m_currToken(0)
{
	
}

void LexerTester::Notify(const Token& token)
{
	const bool isTypeSame = m_expectedTokens[m_currToken].Type == token.Type;
	const bool isValueSame = m_expectedTokens[m_currToken].Value == token.Value;
	const bool isLineSame = m_expectedTokens[m_currToken].Line == token.Line;

	if(!isTypeSame)
		std::wcerr << L"Type mismatch! Expected: " << m_expectedTokens[m_currToken].Type << L". Got: " << token.Type << L'\n';

	if (!isValueSame)
		std::wcerr << L"Value mismatch! Expected: " << m_expectedTokens[m_currToken].Value << L". Got: " << token.Value << L'\n';

	if (!isLineSame)
		std::wcerr << L"Line mismatch! Expected: " << m_expectedTokens[m_currToken].Line << L". Got" << token.Line << L'\n';
	
	BOOST_TEST(isTypeSame);
	BOOST_TEST(isValueSame);
	BOOST_TEST(isLineSame);

	m_currToken++;
}
