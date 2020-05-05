#include "stdafx.h"
#include "ParserStage.h"
#include "ParserElements/Scope.h"
#include "CompilerParts/ParserHelper.h"
#include "CompilerParts/IdentificatorTable.h"

ParserStage::ParserStage(const std::wstring& directoryConfigPath)
	: m_needCreateNewNode(true)
{
	m_root = std::make_shared<Scope>();
	m_root->SetScopeName(L"l0m0");

	IdentificatorTable::GetInstance().AddScope(L"Root", L"l0m0");
}

void ParserStage::DoStage()
{
}

void ParserStage::Notify(const Token& token)
{
	if(token == LexerStage::FinalToken)
	{
		if (!m_currNode->IsComplete()) m_currNode->Compute(token);
		
		ASSERT2(m_root->m_childs.back()->IsComplete(), L"Unexpected end of tokens!");
		
		for (auto& observer : m_observers)
			observer->Notify(m_root.get());

		return;
	}

	if (m_needCreateNewNode)
	{
		if (token.Type == L"CommonSeparator") return;
		
		m_root->m_childs.emplace_back(ParserHelper::CreateNewNodeFromToken(token));
		m_currNode = m_root->m_childs.back();
		m_currNode->parent = m_root.get();
		m_currNode->SetScopeName(std::wstring(L"l0m0"));
	}
	
	m_currNode->Compute(token);

	while (m_currNode->NeedRecompute()) m_currNode->Compute(token);
	
	m_needCreateNewNode = m_currNode->IsComplete();
}
