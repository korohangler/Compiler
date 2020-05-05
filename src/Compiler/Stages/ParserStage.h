#pragma once
#include "IStage.h"
#include "CompilerParts/Observers/INewLexerTokenObserver.h"
#include "CompilerParts/Observers/INewParserTreeObserver.h"

class __declspec(dllexport) ParserStage final : public IStage, public INewLexerTokenObserver
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

	bool m_needCreateNewNode;
	
	std::shared_ptr<AbstractTreeNode> m_root;
	std::shared_ptr<AbstractTreeNode> m_currNode;

	std::vector<INewParserTreeObserver*> m_observers;
};