#pragma once
#include "BasicOperation.h"

class MinusOperation : public BasicOperation
{
public:
	~MinusOperation() override = default;

	[[nodiscard]] OperationType GetOperationType() const override { return OperationType::Binary; }
};
