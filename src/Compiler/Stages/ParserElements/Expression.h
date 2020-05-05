#pragma once
#include "MathOperations/IOperation.h"
#include "Stages/ParserElements/BaseNode.h"

class Expression : public BaseNode
{
public:
	void Compute(const Token& token) override;
	
private:

	[[nodiscard]] static std::shared_ptr<AbstractTreeNode> ParseExpression(std::vector<Token>::const_iterator begin, std::vector<Token>::const_iterator end);

	[[nodiscard]] static std::vector<Token>::const_iterator GetMinPriorityToken(std::vector<Token>::const_iterator begin, std::vector<Token>::const_iterator end);

	[[nodiscard]] static size_t GetOperationPriority(const Token& token);

	[[nodiscard]] static std::shared_ptr<AbstractTreeNode> CreateNode(const Token& token);

	std::vector<Token> m_tokens;
};
