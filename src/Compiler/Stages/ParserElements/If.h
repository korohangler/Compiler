#pragma once
#include "CompilerParts/StageOutputStructs.h"

class If : public AbstractTreeNode
{
public:
	bool IsComplete() const override { return m_isComplete; }

	void Compute(const Token& token) override;

	bool NeedRecompute() const override;

	const std::wstring& GetScopeName() override { return m_scopeName; }

	void SetScopeName(const std::wstring& name) override { m_scopeName = name; }
	
private:

	bool m_needRecompute = false;
	bool m_isComplete	 = false;

	size_t m_counter = 0;

	std::wstring m_scopeName;
};
