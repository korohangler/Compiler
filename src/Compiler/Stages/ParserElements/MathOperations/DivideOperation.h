#pragma once
#include "BasicOperation.h"

class __declspec(dllexport) DivideOperation : public BasicOperation
{
public:
	~DivideOperation() override = default;

	[[nodiscard]] OperationType GetOperationType() const override { return OperationType::Binary; }
};
