#pragma once
#include "Stages/ParserElements/BaseNode.h"

class __declspec(dllexport) Let : public BaseNode
{
public:
	/// AbstractTreeNode override
	void Compute(const Token& token) override;

	[[nodiscard]] const std::wstring& GetSerializeData() const override { return m_variableName; }
	///

	[[nodiscard]] const std::wstring& GetVariableName() const { return m_variableName; }
	
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
	
	std::wstring m_variableName;

	States m_state = States::LetKeyword;
};