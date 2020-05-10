#pragma once
#include "StageOutputStructs/AbstractTreeNode.h"

class __declspec(dllexport) Identificator : public AbstractTreeNode
{
public:
	Identificator() = delete;
	Identificator(std::wstring name) : m_variableName(std::move(name)) {}

	/// AbstractTreeNode override
	[[nodiscard]] bool IsComplete() const override { return !m_variableName.empty(); }

	void Compute(const Token& token) override;

	[[nodiscard]] bool NeedRecompute() const override { return false; }

	[[nodiscard]] const std::wstring& GetSerializeData() const override { return m_variableName; }
	///

	[[nodiscard]] const std::wstring& GetVariableName() const { return m_variableName; }
	
private:

	std::wstring m_variableName;
};
