#pragma once
#include "CompilerParts/StageOutputStructs.h"

class Scope : public AbstractTreeNode
{
public:

	bool IsComplete() const override { return m_isCompleted; }

	void Compute(const Token& token) override;

	bool NeedRecompute() const override { return false; }

	const std::wstring& GetScopeName() override { return m_scopeName; }

	void SetScopeName(const std::wstring& name) override { m_scopeName = name; }

	const std::wstring& GetSerializeData() const override { return m_scopeName; }
	
private:

	std::wstring m_scopeName;
	
	bool m_isCompleted = false;

	bool m_isFirstToken = true;
};