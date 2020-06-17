#pragma once
#include "Stages/ParserElements/BaseNode.h"

class __declspec(dllexport) Expression : public BaseNode
{
public:
	/// AbstractTreeNode override
	void Compute(const Token& token) override;
	///

	[[nodiscard]] std::shared_ptr<AbstractTreeNode> GetRoot() { return Childs[0]; }

	
private:

	struct ExpressionNode
	{
		std::shared_ptr<AbstractTreeNode> node;
		std::wstring Value;
		std::wstring Type;
	};

	void ConvertTokensToExpressionNodes();

	[[nodiscard]] static std::shared_ptr<AbstractTreeNode> ParseExpression(std::vector<ExpressionNode>::const_iterator begin, std::vector<ExpressionNode>::const_iterator end);

	[[nodiscard]] static std::vector<ExpressionNode>::const_iterator GetMinPriorityToken(std::vector<ExpressionNode>::const_iterator begin, std::vector<ExpressionNode>::const_iterator end);

	[[nodiscard]] static size_t GetOperationPriority(const ExpressionNode& token);

	[[nodiscard]] static ExpressionNode CreateNode(const Token& token);

	std::vector<Token> m_tokens;
	std::vector<ExpressionNode> m_convertedTokens;
};
