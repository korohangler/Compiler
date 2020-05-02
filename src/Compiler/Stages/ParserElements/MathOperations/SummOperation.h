#pragma once
#include "BasicOperation.h"

class SummOperation : public BasicOperation
{
public:
	~SummOperation() override = default;

	[[nodiscard]] OperationType GetOperationType() const override { return OperationType::Binary; }
};
