#pragma once
#include "Stages/ParserElements/BaseNode.h"

class If : public BaseNode
{
public:
	void Compute(const Token& token) override;
	
private:
	size_t m_counter = 0;

};
