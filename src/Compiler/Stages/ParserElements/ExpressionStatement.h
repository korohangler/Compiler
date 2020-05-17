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

	[[nodiscard]] const std::shared_ptr<Identificator> GetIdentificator() { return std::static_pointer_cast<Identificator>(Childs[0]); }

	[[nodiscard]] const std::shared_ptr<Expression> GetExpression() { return std::static_pointer_cast<Expression>(Childs.back()); }
	
	[[nodiscard]] const std::vector<std::shared_ptr<Identificator>>& GetArguments() { return m_arguments; }

	[[nodiscard]] StatementType GetType() const { return m_type; }

private:

	std::vector<std::shared_ptr<Identificator>> m_arguments;

	enum class States
	{
		AwaitIdentificator,
		TypeChoise,
		Argument,
		Comma,
		ExpressionComputation,
		SemicolonAwait
	};
	
	void HandleAwaitIdentificatorState(const Token& token);
	void HandleTypeChoiseState(const Token& token);
	void HandleExpressionComputingState(const Token& token);
	void HandleSemicolonAwaitState(const Token& token);
	void HandleArgument(const Token& token);
	void HandleComma(const Token& token);

	States m_state = States::AwaitIdentificator;

	StatementType m_type = StatementType::AssignmentExpression;
};