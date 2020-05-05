#pragma once
#include "CompilerParts/StageOutputStructs.h"
#include "Stages/ParserElements/BaseNode.h"

class __declspec(dllexport) ExpressionStatement : public BaseNode
{
public:
	~ExpressionStatement() override = default;

	/// AbstractTreeNode override
	void Compute(const Token& token) override;
	///

	enum class StatementType
	{
		CallExpression
		, AssignmentExpression
	};

	[[nodiscard]] const std::wstring& GetIdentificatorName() const { return m_identificator; }
	
private:

	size_t m_counter = 0;

	StatementType m_type = StatementType::AssignmentExpression;

	std::wstring m_identificator; // function name or variable name
};