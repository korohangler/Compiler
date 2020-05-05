#pragma once
#include "BasicOperation.h"

class __declspec(dllexport) SummOperation : public BasicOperation
{
public:
	~SummOperation() override = default;

	[[nodiscard]] OperationType GetOperationType() const override { return OperationType::Binary; }
};
