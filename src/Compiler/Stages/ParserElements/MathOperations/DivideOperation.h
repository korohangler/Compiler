#pragma once
#include "BasicOperation.h"

class __declspec(dllexport) DivideOperation : public BasicOperation
{
public:
	~DivideOperation() override = default;

	/// IOperation override
	[[nodiscard]] OperationType GetOperationType() const override { return OperationType::Binary; }
	///
};
