#pragma once
#include "Stages/ParserElements/BaseNode.h"

class __declspec(dllexport) Let : public BaseNode
{
public:
	/// AbstractTreeNode override
	void Compute(const Token& token) override;

	[[nodiscard]] const std::wstring& GetSerializeData() const override { return m_variableName; }
	///

	[[nodiscard]] const std::wstring& GetVariableName() const { return m_variableName; }
	
private:
	std::wstring m_variableName;

	size_t m_counter = 0;
};