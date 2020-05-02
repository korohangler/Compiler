#pragma once
#include "BasicOperation.h"

class DivideOperation : public BasicOperation
{
public:
	~DivideOperation() override = default;

	[[nodiscard]] OperationType GetOperationType() const override { return OperationType::Binary; }
};
