#pragma once
#include "Stages/ParserElements/BaseNode.h"

class __declspec(dllexport) While : public BaseNode
{
public:
	~While() override = default;

	/// AbstractTreeNode override
	void Compute(const Token& token) override;
	///

private:

	enum class States
	{
		KeywordWhile,
		ExpressionCreation,
		ExpressionComputation,
		ScopeCreation,
		ScopeComputation
	};

	void HandleKeywordWhile(const Token& token);
	void HandleExpressionCreation();
	void HandleExpressionComputation(const Token& token);
	void HandleScopeCreation(const Token& token);
	void HandleScopeComputation(const Token& token);
	
	std::wstring m_functionName;

	States m_state = States::KeywordWhile;
};