#include "stdafx.h"
#include "Scope.h"
#include "CompilerParts/ParserHelper.h"

void Scope::Compute(const Token& token)
{
	switch(m_state)
	{
	case States::OpenBracket:
		HandleOpenBracket(token);
		break;
	case States::ComputeChildsOrBracket:
		HandleComputation(token);
		break;
	default:
		Utils::ASSERT(std::string("Unknown state! At line: ") + std::to_string(__LINE__) + std::string(". In file: ") + __FILE__);
	}
}

void Scope::HandleOpenBracket(const Token& token)
{
	if (token.Type == L"CommonSeparator") return;

	Utils::ASSERT2(token.Type == L"Bracket" && token.Value == L"{",
		std::wstring(L"token type mismatch! Expected CurlyOpeningBracket, got ") + token.Type + std::wstring(L". At line: ") + std::to_wstring(token.Line));

	m_state = States::ComputeChildsOrBracket;
}

void Scope::HandleComputation(const Token& token)
{
	if (token.Type == L"Bracket" && token.Value == L"}" && (m_childs.empty() || m_childs.back()->IsComplete()))
	{
		m_needRecompute = false;
		m_isComplete = true;
	}
	else
	{
		if (token.Type != L"CommonSeparator" && (m_childs.empty() || m_childs.back()->IsComplete()))
		{
			m_childs.emplace_back(ParserHelper::CreateNewNodeFromToken(token));
			m_childs.back()->parent = this;
		}

		if (!m_childs.empty())
		{
			if (!m_childs.back()->IsComplete())
				m_childs.back()->Compute(token);

			m_needRecompute = m_childs.back()->NeedRecompute();
		}
	}
}
