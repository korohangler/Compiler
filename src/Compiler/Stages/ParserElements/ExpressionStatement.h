#pragma once
#include "CompilerParts/StageOutputStructs.h"

class ExpressionStatement : public AbstractTreeNode
{
public:
	~ExpressionStatement() override = default;

	[[nodiscard]] bool IsComplete() const override { return m_isComplete; }

	void Compute(const Token& token) override;

	[[nodiscard]] bool NeedRecompute() const override { return m_needRecompute; }

	[[nodiscard]] const std::wstring& GetScopeName() override { return parent->GetScopeName(); }

	void SetScopeName(const std::wstring& name) override {}

	[[nodiscard]] const std::wstring& GetSerializeData() const override { return m_serializeData; }

	enum class StatementType
	{
		CallExpression
		, AssignmentExpression
	};
	
private:

	bool m_isComplete = false;

	bool m_needRecompute = false;

	std::wstring m_serializeData;

	size_t m_counter = 0;

	StatementType m_type = StatementType::AssignmentExpression;

	std::wstring m_identificator; // function name or variable name
};