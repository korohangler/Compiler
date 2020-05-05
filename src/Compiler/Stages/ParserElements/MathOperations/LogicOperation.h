#pragma once
#include "BasicOperation.h"


class __declspec(dllexport) LogicOperation : public BasicOperation
{
public:
	LogicOperation() = delete;
	LogicOperation(const Token& token);

	enum class Operation
	{
		More
		, Less
		, Equal
		, EqualTypeCheck
		, MoreOrEqual
		, LessOrEqual
		, NotEqual
		, NotEqualTypeCheck
		, Or
		, And
		, Not
		, Unknown
	};

	/// AbstractTreeNode override
	[[nodiscard]] const std::wstring& GetSerializeData() const override { return m_serializationData; }
	///

	/// IOperation override
	[[nodiscard]] OperationType GetOperationType() const override { return m_operationType; }
	///
	
private:

	Operation m_type = Operation::Unknown;

	OperationType m_operationType = OperationType::Binary;

	std::wstring m_serializationData;
};