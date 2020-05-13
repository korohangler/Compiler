#pragma once
#include "Stages/ParserElements/BaseNode.h"
#include "Stages\ParserElements\Identificator.h"

class __declspec(dllexport) Let : public BaseNode
{
public:
	/// AbstractTreeNode override
	void Compute(const Token& token) override;

	[[nodiscard]] const std::wstring_view GetSerializeData() const override { return std::static_pointer_cast<Identificator>(Childs[0])->GetVariableName(); }
	///

	[[nodiscard]] std::shared_ptr<Identificator> GetVariable() const { return std::static_pointer_cast<Identificator>(Childs[0]); }
	
private:

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