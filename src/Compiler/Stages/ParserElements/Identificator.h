#pragma once
#include "CompilerParts/StageOutputStructs.h"

class Identificator : public AbstractTreeNode
{
public:
	Identificator() = delete;
	Identificator(const std::wstring& name) : m_variableName(name) {}

	bool IsComplete() const override { return !m_variableName.empty(); }

	void Compute(const Token& token) override;

	bool NeedRecompute() const override { return false; }

	const std::wstring& GetScopeName() override { return m_scopeName; }

	void SetScopeName(const std::wstring& name) override { m_scopeName = name; }

	const std::wstring& GetSerializeData() const override { return m_variableName; }
	
private:

	std::wstring m_variableName;
	
	std::wstring m_scopeName;
};
