#pragma once
#include "IOperation.h"

class __declspec(dllexport) BasicOperation : public IOperation
{
public:
	~BasicOperation() override = default;
	BasicOperation() : IOperation(2) { }
	
	[[nodiscard]] std::shared_ptr<AbstractTreeNode> GetRight() const override { return m_childs[1]; }
	[[nodiscard]] std::shared_ptr<AbstractTreeNode> GetLeft() const override { return m_childs[0]; }

	void SetRight(std::shared_ptr<AbstractTreeNode> right) override { m_childs[1] = right; }
	void SetLeft(std::shared_ptr<AbstractTreeNode> left) override { m_childs[0] = left; }

	[[nodiscard]] bool IsComplete() const override { return true; }
	void Compute(const Token& token) override {}
	[[nodiscard]] bool NeedRecompute() const override { return false; }

	[[nodiscard]] const std::wstring& GetSerializeData() const override { return m_data; }

private:

	std::wstring m_data;
};
