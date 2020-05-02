#pragma once
#include "CompilerParts/StageOutputStructs.h"

class For : public AbstractTreeNode
{
public:
	bool IsComplete() const override;

	void Compute(const Token& token) override;

private:

	size_t m_counter = 0;
};
