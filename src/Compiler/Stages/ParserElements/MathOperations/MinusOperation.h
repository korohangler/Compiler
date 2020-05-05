#pragma once
#include "BasicOperation.h"

class __declspec(dllexport) MinusOperation : public BasicOperation
{
public:
	~MinusOperation() override = default;

	/// IOperation override
	[[nodiscard]] OperationType GetOperationType() const override { return OperationType::Binary; }
	///
};
