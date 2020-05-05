#pragma once
#include "Stages/ParserElements/BaseNode.h"

class Function : public BaseNode
{
public:
	~Function() override = default;
	
	void Compute(const Token& token) override;

	[[nodiscard]] const std::wstring& GetFunctionName() const { return m_functionName; }
	
private:

	std::wstring m_functionName;

	size_t m_counter = 0;
};
