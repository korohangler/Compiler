#pragma once
#include "StageOutputStructs/AbstractTreeNode.h"

class __declspec(dllexport) IOperation
{
public:
	virtual ~IOperation() = default;
	IOperation() = default;

	[[nodiscard]] virtual std::shared_ptr<AbstractTreeNode> GetLeft() const = 0;

	[[nodiscard]] virtual std::shared_ptr<AbstractTreeNode> GetRight() const = 0;

	virtual void SetLeft(std::shared_ptr<AbstractTreeNode> left) = 0;

	virtual void SetRight(std::shared_ptr<AbstractTreeNode> right) = 0;

	enum class OperationType
	{
		Unary
		, Binary
	};

	[[nodiscard]] virtual OperationType GetOperationType() const = 0;
};