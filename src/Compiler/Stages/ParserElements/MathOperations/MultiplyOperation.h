#pragma once
#include "BasicOperation.h"

class __declspec(dllexport) MultiplyOperation : public BasicOperation
{
public:
	~MultiplyOperation() override = default;

	[[nodiscard]] OperationType GetOperationType() const override { return OperationType::Binary; }
};
