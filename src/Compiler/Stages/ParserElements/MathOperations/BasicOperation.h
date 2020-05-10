#pragma once
#include "IOperation.h"
#include "StageOutputStructs/AbstractTreeNode.h"

class __declspec(dllexport) BasicOperation : public IOperation, public AbstractTreeNode
{
public:
	~BasicOperation() override = default;
	BasicOperation() : AbstractTreeNode(2) { }

	/// IOperation override
	[[nodiscard]] std::shared_ptr<AbstractTreeNode> GetRight() const override { return Childs[1]; }
	[[nodiscard]] std::shared_ptr<AbstractTreeNode> GetLeft() const override { return Childs[0]; }

	void SetRight(std::shared_ptr<AbstractTreeNode> right) override { Childs[1] = right; }
	void SetLeft(std::shared_ptr<AbstractTreeNode> left) override { Childs[0] = left; }
	///

	/// AbstractTreeNode override
	[[nodiscard]] bool IsComplete() const override { return true; }
	void Compute(const Token& token) override {}
	[[nodiscard]] bool NeedRecompute() const override { return false; }

	[[nodiscard]] const std::wstring& GetSerializeData() const override { return m_data; }
	///

private:

	std::wstring m_data;
};
