#pragma once
#include "Stages/ParserElements/BaseNode.h"
#include "Stages/ParserElements/Identificator.h"
#include "Stages/ParserElements/Expression.h"

class __declspec(dllexport) Let : public BaseNode
{
public:
	Let() : m_hasInitialization(false) {}
	~Let() override = default;

	/// AbstractTreeNode override
	void Compute(const Token& token) override;

	[[nodiscard]] const std::wstring_view GetSerializeData() const override { return std::static_pointer_cast<Identificator>(Childs[0])->GetName(); }
	///

	[[nodiscard]] std::shared_ptr<Identificator> GetVariable() const { return std::static_pointer_cast<Identificator>(Childs[0]); }
	
	[[nodiscard]] bool HasInitializationExpression() const { return m_hasInitialization; }

	[[nodiscard]] std::shared_ptr<Expression> GetExpression() { return std::static_pointer_cast<Expression>(Childs[1]); }

private:

	bool m_hasInitialization;

	enum class States
	{
		LetKeyword,
		CommonSeparator,
		Identificator,
		SemicolonOrAssign,
		ExpressionComputation,
		Semicolon
	};

	void HandleKeyWord(const Token& token);
	void HandleCommonSeparator(const Token& token);
	void HandleIdentificator(const Token& token);
	void HandleSemicolonOrAssign(const Token& token);
	void HandleExpressionComputation(const Token& token);
	void HandleSemicolon(const Token& token);

	States m_state = States::LetKeyword;
};