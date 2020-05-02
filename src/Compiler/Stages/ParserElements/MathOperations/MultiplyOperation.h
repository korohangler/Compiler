#pragma once
#include "BasicOperation.h"

class MultiplyOperation : public BasicOperation
{
public:
	~MultiplyOperation() override = default;

	[[nodiscard]] OperationType GetOperationType() const override { return OperationType::Binary; }
};
