#include "stdafx.h"
#include "LogicOperation.h"

LogicOperation::LogicOperation(const Token& token)
{
	if (token.Value == L"<")
	{
		m_type = Operation::Less;
		m_serializationData = L"Less";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L">")
	{
		m_type = Operation::More;
		m_serializationData = L"More";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"==")
	{
		m_type = Operation::Equal;
		m_serializationData = L"Equal";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"===")
	{
		m_type = Operation::EqualTypeCheck;
		m_serializationData = L"EqualTypeCheck";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L">=")
	{
		m_type = Operation::MoreOrEqual;
		m_serializationData = L"MoreOrEqual";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"<=")
	{
		m_type = Operation::LessOrEqual;
		m_serializationData = L"LessOrEqual";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"!=")
	{
		m_type = Operation::NotEqual;
		m_serializationData = L"NotEqual";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"!==")
	{
		m_type = Operation::NotEqualTypeCheck;
		m_serializationData = L"NotEqualTypeCheck";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"||")
	{
		m_type = Operation::Or;
		m_serializationData = L"Or";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"&&")
	{
		m_type = Operation::And;
		m_serializationData = L"And";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"!")
	{
		m_type = Operation::Not;
		m_serializationData = L"Not";
		m_operationType = OperationType::Unary;
	}
	else
	{
		Utils::ASSERT(L"Expected logic operator! How do you even end up here?");
	}
}
