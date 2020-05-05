#pragma once
#include "Stages/ParserElements/BaseNode.h"

class __declspec(dllexport) While : public BaseNode
{
public:
	~While() override = default;

	/// AbstractTreeNode override
	void Compute(const Token& token) override;
	///

private:
	std::wstring m_functionName;

	size_t m_counter = 0;
};