#pragma once
#include "CompilerParts/StageOutputStructs.h"

class While : public AbstractTreeNode
{
	bool IsComplete() const override;

	void Compute(const Token& token) override;
};