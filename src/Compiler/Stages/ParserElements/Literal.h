#pragma once
#include "CompilerParts/StageOutputStructs.h"

class Literal : public AbstractTreeNode
{
public:
	Literal() = delete;
	Literal(const Token& token);
	~Literal() override = default;

	[[nodiscard]] bool IsComplete() const override { return true; }

	void Compute(const Token& token) override {}

	[[nodiscard]] bool NeedRecompute() const override { return false; }

	const std::wstring& GetScopeName() override { return parent->GetScopeName(); }

	void SetScopeName(const std::wstring& name) override {}

	[[nodiscard]] const std::wstring& GetSerializeData() const override { return m_serializeData; }

	enum class LiteralType
	{
		String,
		Number
	};

private:

	LiteralType m_literalType = LiteralType::String;

	std::wstring m_data;

	std::wstring m_serializeData;
};