#pragma once
#include "BasicOperation.h"


class LogicOperation : public BasicOperation
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

	[[nodiscard]] const std::wstring& GetSerializeData() const override { return m_serializationData; }

	[[nodiscard]] OperationType GetOperationType() const override { return m_operationType; }
	
private:

	Operation m_type = Operation::Unknown;

	OperationType m_operationType = OperationType::Binary;

	std::wstring m_serializationData;
};