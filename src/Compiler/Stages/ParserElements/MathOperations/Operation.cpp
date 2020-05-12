#include "stdafx.h"
#include "Operation.h"

Operation::Operation(const Token& token) : AbstractTreeNode(2)
{
	if (token.Value == L"+")
	{
		m_name = OperationName::Summ;
		m_serializationData = L"Add";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"-")
	{
		m_name = OperationName::Sub;
		m_serializationData = L"Sub";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"/")
	{
		m_name = OperationName::Div;
		m_serializationData = L"Div";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"*")
	{
		m_name = OperationName::Mul;
		m_serializationData = L"Mul";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"<")
	{
		m_name = OperationName::Less;
		m_serializationData = L"Less";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L">")
	{
		m_name = OperationName::More;
		m_serializationData = L"More";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"==")
	{
		m_name = OperationName::Equal;
		m_serializationData = L"Equal";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"===")
	{
		m_name = OperationName::EqualTypeCheck;
		m_serializationData = L"EqualTypeCheck";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L">=")
	{
		m_name = OperationName::MoreOrEqual;
		m_serializationData = L"MoreOrEqual";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"<=")
	{
		m_name = OperationName::LessOrEqual;
		m_serializationData = L"LessOrEqual";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"!=")
	{
		m_name = OperationName::NotEqual;
		m_serializationData = L"NotEqual";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"!==")
	{
		m_name = OperationName::NotEqualTypeCheck;
		m_serializationData = L"NotEqualTypeCheck";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"||")
	{
		m_name = OperationName::Or;
		m_serializationData = L"Or";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"&&")
	{
		m_name = OperationName::And;
		m_serializationData = L"And";
		m_operationType = OperationType::Binary;
	}
	else if (token.Value == L"!")
	{
		m_name = OperationName::Not;
		m_serializationData = L"Not";
		m_operationType = OperationType::Unary;
	}
	else
	{
		Utils::ASSERT(L"Expected logic operator! How do you even end up here?");
	}
}
