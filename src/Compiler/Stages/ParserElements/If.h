#pragma once
#include "Stages/ParserElements/BaseNode.h"

class __declspec(dllexport) If : public BaseNode
{
public:
	/// AbstractTreeNode override
	void Compute(const Token& token) override;
	///
	
private:
	void HandleIfKeyword(const Token& token);
	void HandleOpeningBracket(const Token& token);
	void HandleExpressionComputation(const Token& token);
	void HandleScopeCreation(const Token& token);
	void HandleScopeComputation(const Token& token);
	void HandleElseKeyword(const Token& token);
	void HandleElseIfPossibility(const Token& token);

	enum class States
	{
		IfKeyword,
		OpeningBracket,
		ExpressionComputation,
		ScopeCreation,
		ScopeComputation,
		ElseKeyword,
		ElseIfPossibility
	};
	
	States m_state = States::IfKeyword;

};
