#pragma once
#include "CompilerParts/Observers/Observable.h"
#include "IStage.h"
#include "StageOutputStructs/AbstractTreeNode.h"
#include "ParserElements/Scope.h"

class __declspec(dllexport) ParserStage final : public IStage, public IObserver<const Token&>, public Observable<std::shared_ptr<AbstractTreeNode>>
{
public:
	ParserStage() : m_needCreateNewNode(true), m_root(std::make_shared<Scope>()) {};
	~ParserStage() override = default;

	/// IStage override
	void DoStage() override {}

	[[nodiscard]] std::wstring GetStageName() override { return L"ParserStage"; }
	///

	/// INewLexerTokenObserver override
	void Notify(const Token& token) override;
	///

private:

	bool m_needCreateNewNode;
	
	std::shared_ptr<AbstractTreeNode> m_root;
	std::shared_ptr<AbstractTreeNode> m_currNode;
};
