#pragma once
#include "StageOutputStructs/AbstractTreeNode.h"

enum class OperationType
{
	Binary,
	Unary
};

enum class OperationName
{
	Summ
	, Sub
	, Div
	, Mul
	, Less
	, More
	, Equal
	, Or
	, And
	, EqualTypeCheck
	, MoreOrEqual
	, LessOrEqual
	, NotEqual
	, NotEqualTypeCheck
	, Not
	, Unknown
};

class __declspec(dllexport) Operation : public AbstractTreeNode
{
public:
	Operation() = delete;
	Operation(const Token& token);

	[[nodiscard]] std::shared_ptr<AbstractTreeNode> GetRight() const { return Childs[1]; }
	[[nodiscard]] std::shared_ptr<AbstractTreeNode> GetLeft() const { return Childs[0]; }

	void SetRight(std::shared_ptr<AbstractTreeNode> right) { Childs[1] = right; }
	void SetLeft(std::shared_ptr<AbstractTreeNode> left) { Childs[0] = left; }

	[[nodiscard]] OperationType GetOperationType() const { return m_operationType; }

	/// AbstractTreeNode override
	[[nodiscard]] bool IsComplete() const { return true; }
	void Compute(const Token& token) {}
	[[nodiscard]] bool NeedRecompute() const { return false; }
	[[nodiscard]] const std::wstring& GetSerializeData() const { return m_serializationData; }
	///
	
	OperationName GetOperationName() { return m_name; }

private:

	OperationName m_name = OperationName::Unknown;

	OperationType m_operationType = OperationType::Binary;

	std::wstring m_serializationData;
};
