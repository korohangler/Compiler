#pragma once
#include "CompilerParts/StageOutputStructs.h"

class Expression;

class IOperation : public AbstractTreeNode
{
public:
	virtual ~IOperation() = default;
	IOperation() = default;
	IOperation(size_t childsCount) : AbstractTreeNode(childsCount) {}

	[[nodiscard]] virtual std::shared_ptr<AbstractTreeNode> GetLeft() const = 0;

	[[nodiscard]] virtual std::shared_ptr<AbstractTreeNode> GetRight() const = 0;

	virtual void SetLeft(std::shared_ptr<AbstractTreeNode> left) = 0;

	virtual void SetRight(std::shared_ptr<AbstractTreeNode> right) = 0;

	enum class OperationPriority
	{
		Lowest = 0
		, Low
		, Medium
		, High
		, Highest
		, Unknown
	};

	enum class OperationType
	{
		Unary
		, Binary
	};

	[[nodiscard]] virtual OperationType GetOperationType() const = 0;
};