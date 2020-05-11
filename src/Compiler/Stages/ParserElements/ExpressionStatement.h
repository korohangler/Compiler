#pragma once
#include "StageOutputStructs/AbstractTreeNode.h"
#include "Stages/ParserElements/BaseNode.h"
#include "Stages/ParserElements/Expression.h"
#include "Stages/ParserElements/Identificator.h"

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

	[[nodiscard]] std::shared_ptr<Identificator> GetIdentificator() { return std::static_pointer_cast<Identificator>(Childs[0]); }

	[[nodiscard]] std::shared_ptr<Expression> GetExpression() { return std::static_pointer_cast<Expression>(Childs.back()); }
	
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
};