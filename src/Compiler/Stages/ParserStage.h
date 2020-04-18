#pragma once
#include "IStage.h"
#include "CompilerParts/Observers/INewLexerTokenObserver.h"
#include "CompilerParts/Observers/INewParserTreeObserver.h"
#include "CompilerParts/StageOutputStructs.h"

class ParserStage final : public IStage, public INewLexerTokenObserver
{
public:
	ParserStage(const std::wstring& directoryConfigPath);
	~ParserStage() = default;
	void DoStage() override;

	/// INewLexerTokenObserver override
	void Notify(const Token& token) override;
	///

	void RegisterListener(INewParserTreeObserver* observer) { m_observers.push_back(observer); }
	void UnRegisterListener(INewParserTreeObserver* observer) { m_observers.erase(std::find(m_observers.begin(), m_observers.end(), observer)); }
	
	std::wstring GetStageName() override { return L"ParserStage"; }

private:

	struct ParserState
	{
		struct TokenRule
		{
			bool addToIdentificatorTable = false;
			std::wstring nextState;

			std::wstring type;
			
			bool descend = false;
			bool ascend = false;
			bool addToTree = false;
		};
		
		std::unordered_map<std::wstring, TokenRule> TokenToStageMap;

		std::wstring stateName;
	};

	AbstractTreeNode* m_root;

	AbstractTreeNode* m_currNode;

	static std::vector<ParserState> CreateNewStates(WDocument& doc);

	std::wstring m_currentState = L"MainState";

	std::unordered_map<std::wstring, ParserState> m_states;

	std::vector<INewParserTreeObserver*> m_observers;
};