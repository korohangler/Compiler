#pragma once
#include "Stages/ParserElements/BaseNode.h"

class __declspec(dllexport) Scope : public BaseNode
{
public:
	/// AbstractTreeNode override
	void Compute(const Token& token) override;
	///
	
private:

	size_t m_counter = 0;
};