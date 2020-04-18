#include "stdafx.h"
#include "ParserStage.h"

ParserStage::ParserStage(const std::wstring& directoryConfigPath)
{

	for(std::filesystem::directory_iterator dirIt(directoryConfigPath); !dirIt._At_end(); dirIt++)
	{
		if (dirIt->is_directory()) continue;

		std::wifstream docInp(*dirIt);
		rapidjson::WIStreamWrapper wrappedInp(docInp);
		
		WDocument doc;
		doc.ParseStream(wrappedInp);

		std::vector<ParserState> newStates = CreateNewStates(doc);

		for (auto& state : newStates)
			m_states.emplace(state.stateName, state);
	}

	m_root = new AbstractTreeNode();
	m_root->type = L"Root";

	m_currNode = m_root;
}

void ParserStage::DoStage()
{
}

void ParserStage::Notify(const Token& token)
{
	if(token == LexerStage::FinalToken)
	{
		for (auto& observer : m_observers)
			observer->Notify(m_root);

		return;
	}
	
	ParserState& currState = m_states[m_currentState];

	ParserState::TokenRule& currRule = currState.TokenToStageMap[token.Type];

	m_currentState = currRule.nextState;
	
	if (currRule.addToTree)
	{
		AbstractTreeNode newNode;
		newNode.parent = m_currNode;
		newNode.type = currRule.type;
		newNode.data = token.Value;

		m_currNode->m_childs.emplace_back(std::move(newNode));

		if (currRule.descend)
			m_currNode = &(m_currNode->m_childs.back());
		else if (currRule.ascend)
			m_currNode = m_currNode->parent;
	}
}

std::vector<ParserStage::ParserState> ParserStage::CreateNewStates(WDocument& doc)
{
	std::vector<ParserState> newStates;
	
	if (doc.HasMember(L"States"))
	{
		auto states = doc[L"States"].GetArray();

		for (auto& state : states)
		{
			ParserState newState;

			newState.stateName = state.HasMember(L"StateName") ? state[L"StateName"].GetString() : L"";

			if (state.HasMember(L"StateModifiers"))
			{
				auto stateModifiers = state[L"StateModifiers"].GetArray();

				for (auto& modifier : stateModifiers)
				{
					std::wstring tokenType = modifier.HasMember(L"Type") ? modifier[L"Type"].GetString() : L"";

					ParserState::TokenRule newRule;

					newRule.addToIdentificatorTable = modifier.HasMember(L"AddToIdentificatorTable") ? modifier[L"AddToIdentificatorTable"].GetBool() : false;

					newRule.nextState = modifier.HasMember(L"NextState") ? modifier[L"NextState"].GetString() : L"";

					newRule.type = modifier.HasMember(L"Type") ? modifier[L"Type"].GetString() : L"";

					newRule.descend = modifier.HasMember(L"Descend") ? modifier[L"Descend"].GetBool() : false;
					newRule.ascend = modifier.HasMember(L"Ascend") ? modifier[L"Ascend"].GetBool() : false;

					newRule.addToTree = modifier.HasMember(L"AddToTree") ? modifier[L"AddToTree"].GetBool() : false;

					newState.TokenToStageMap.emplace(std::move(tokenType), std::move(newRule));
				}
			}

			newStates.push_back(std::move(newState));
		}
	}

	return newStates;
}
