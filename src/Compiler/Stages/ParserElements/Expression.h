#pragma once
#include "CompilerParts/StageOutputStructs.h"
#include "MathOperations/IOperation.h"

class Expression : public AbstractTreeNode
{
public:
	bool IsComplete() const override { return m_isComplete; }

	void Compute(const Token& token) override;

	bool NeedRecompute() const override { return m_needRecompute; }

	const std::wstring& GetScopeName() override { return parent->GetScopeName(); }

	void SetScopeName(const std::wstring& name) override {}

	const std::wstring& GetSerializeData() const override { return m_serializeData; }
	
private:

	[[nodiscard]] static std::shared_ptr<AbstractTreeNode> ParseExpression(std::vector<Token>::const_iterator begin, std::vector<Token>::const_iterator end);

	[[nodiscard]] static std::vector<Token>::const_iterator GetMinPriorityToken(std::vector<Token>::const_iterator begin, std::vector<Token>::const_iterator end);

	[[nodiscard]] static size_t GetOperationPriority(const Token& token);

	[[nodiscard]] static std::shared_ptr<AbstractTreeNode> CreateNode(const Token& token);
	
	std::wstring m_serializeData;

	std::vector<Token> m_tokens;

	bool m_needRecompute = false;

	bool m_isComplete = false;
};
