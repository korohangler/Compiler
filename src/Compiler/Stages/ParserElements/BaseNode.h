#pragma once
#include "StageOutputStructs/AbstractTreeNode.h"

class __declspec(dllexport) BaseNode : public AbstractTreeNode
{
public:
	/// AbstractTreeNode override
	[[nodiscard]] bool IsComplete() const override { return m_isComplete; }

	void Compute(const Token& token) override = 0;

	[[nodiscard]] bool NeedRecompute() const override { return m_needRecompute; }

	[[nodiscard]] const std::wstring_view GetSerializeData() const override { return m_serializeData; }
	///

protected:
	std::wstring m_serializeData;

	bool m_needRecompute = false;

	bool m_isComplete = false;
};
