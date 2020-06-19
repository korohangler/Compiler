#pragma once
#include "Stages/ParserElements/BaseNode.h"

class __declspec(dllexport) Function : public BaseNode
{
public:
	Function() : m_argumentsCount(0) {}

	~Function() override = default;

	/// AbstractTreeNode override
	void Compute(const Token& token) override;
	///

	[[nodiscard]] const std::wstring_view GetFunctionName() const { return m_functionName; }
	
	size_t GetArgumentsCount() const { return m_argumentsCount; }

private:
	enum class States
	{
		FunctionKeyword,
		Identificator,
		OpeningBracket,
		ArgumentOrBracket,
		CommaOrBracket,
		ScopeCreation,
		ScopeComputation
	};
	
	void HandleFunctionKeyword(const Token& token);
	void HandleIdentificator(const Token& token);
	void HandleOpeningBracket(const Token& token);
	void HandleArgumentOrBracket(const Token& token);
	void HandleCommaOrBracket(const Token& token);
	void HandleScopeCreation(const Token& token);
	void HandleScopeComputation(const Token& token);

	std::wstring m_functionName;

	size_t m_argumentsCount;

	States m_state = States::FunctionKeyword;
};
