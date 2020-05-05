#pragma once
#include "BasicOperation.h"

class __declspec(dllexport) MultiplyOperation : public BasicOperation
{
public:
	~MultiplyOperation() override = default;

	/// IOperation override
	[[nodiscard]] OperationType GetOperationType() const override { return OperationType::Binary; }
	///
};
