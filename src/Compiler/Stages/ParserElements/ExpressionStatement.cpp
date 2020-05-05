#include "stdafx.h"
#include "ExpressionStatement.h"
#include "Expression.h"

void ExpressionStatement::Compute(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;
	
	if (m_counter == 0)
	{
		Utils::ASSERT2(token.Type == L"Identificator",
		               std::wstring(L"Token type mismatch! Expected: Identificator. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_identificator = token.Value;
		
		m_counter++;
	}
	else if (m_counter == 1)
	{
		if (token.Type == L"Bracket" && token.Value == L"(")
		{
			m_type = StatementType::CallExpression;
			
			m_counter = 4;
		}
		else if (token.Type == L"Assignment")
		{
			m_type = StatementType::AssignmentExpression;
			
			// Simple expression
			m_childs.push_back(std::make_shared<Expression>());

			m_counter++;
		}

		if (m_type == StatementType::AssignmentExpression)
			m_serializeData = L"AssignmentExpression: ";
		else if (m_type == StatementType::CallExpression)
			m_serializeData = L"CallExpression: ";

		m_serializeData += m_identificator;
	}
	else if (m_counter == 2)
	{
		m_childs.back()->Compute(token);

		if (m_childs.back()->IsComplete()) m_counter++;

		m_needRecompute = m_childs.back()->NeedRecompute();
	}
	else if(m_counter == 3)
	{
		Utils::ASSERT2(token.Type == L"Semicolon",
		               std::wstring(L"Token type mismatch! Expected: Semicolon. Got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_isComplete = true;
		m_needRecompute = false;
	}
	else if(m_counter == 4)
	{
		Utils::ASSERT2(token.Type == L"Bracket" && token.Value == L")",
		               std::wstring(L"Token type mismatch! Expected Bracket. But got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_counter++;
	}
	else if(m_counter == 5)
	{
		Utils::ASSERT2(token.Type == L"Semicolon",
		               std::wstring(L"Token type mismatch! Expected Semicolon. But got: ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

		m_isComplete = true;
		m_needRecompute = false;
		
		m_counter++;
	}
}
