#pragma once
#include "CompilerParts/StageOutputStructs.h"

class Let : public AbstractTreeNode
{
public:

	bool IsComplete() const override;

	void Compute(const Token& token) override;

	bool NeedRecompute() const override { return m_needRecompute; }

	const std::wstring& GetScopeName() override { return m_scopeName; }

	void SetScopeName(const std::wstring& name) override { m_scopeName = name; }

	const std::wstring& GetSerializeData() const override { return m_variableName; };
	
private:

	std::wstring m_variableName;
	
	std::wstring m_scopeName;
	
	bool m_isComplete = false;

	bool m_needRecompute = false;

	size_t m_counter = 0;
};