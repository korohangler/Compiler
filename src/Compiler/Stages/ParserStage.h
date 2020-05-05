#pragma once
#include "CompilerParts/Observers/IObservable.h"
#include "IStage.h"
#include "CompilerParts/StageOutputStructs.h"

class __declspec(dllexport) ParserStage final : public IStage, public IObserver<const Token&>, public IObservable<IObserver<std::shared_ptr<AbstractTreeNode>>, std::shared_ptr<AbstractTreeNode>>
{
public:
	ParserStage();
	~ParserStage() override = default;

	/// IStage override
	void DoStage() override;

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