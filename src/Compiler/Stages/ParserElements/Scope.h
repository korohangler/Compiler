#pragma once
#include "Stages/ParserElements/BaseNode.h"

class __declspec(dllexport) Scope : public BaseNode
{
public:
	/// AbstractTreeNode override
	void Compute(const Token& token) override;
	///
	
private:

	enum class States
	{
		OpenBracket,
		ComputeChildsOrBracket
	};

	void HandleOpenBracket(const Token& token);
	void HandleComputation(const Token& token);

	States m_state = States::OpenBracket;
};