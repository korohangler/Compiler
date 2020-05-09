#pragma once
#include "CompilerParts/StageOutputStructs.h"
#include "Stages/ParserElements/BaseNode.h"
#include "Stages/ParserElements/Expression.h"

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

	[[nodiscard]] std::shared_ptr<Expression> GetExpression() { return std::dynamic_pointer_cast<Expression>(m_childs.back()); }
	
private:

	enum class States
	{
		AwaitIdentificator,
		TypeChoise,
		ExpressionComputation,
		SemicolonAwait,
		AwaitCloseBracket
	};
	
	void HandleAwaitIdentificatorState(const Token& token);
	void HandleTypeChoiseState(const Token& token);
	void HandleExpressionComputingState(const Token& token);
	void HandleSemicolonAwaitState(const Token& token);
	void HandleClosingBracketAwaitState(const Token& token);

	States m_state = States::AwaitIdentificator;

	StatementType m_type = StatementType::AssignmentExpression;

	std::wstring m_identificator; // function name or variable name
};