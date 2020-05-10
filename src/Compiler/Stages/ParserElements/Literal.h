#pragma once
#include "StageOutputStructs/AbstractTreeNode.h"

class __declspec(dllexport) Literal : public AbstractTreeNode
{
public:
	Literal() = delete;
	Literal(const Token& token);
	~Literal() override = default;

	/// AbstractTreeNode override
	[[nodiscard]] bool IsComplete() const override { return true; }

	void Compute(const Token& token) override {}

	[[nodiscard]] bool NeedRecompute() const override { return false; }

	[[nodiscard]] const std::wstring& GetSerializeData() const override { return m_serializeData; }
	///

	[[nodiscard]] const std::wstring& GetData() const { return m_data; }

	[[nodiscard]] float GetFloatData() const { return _wtof(m_data.c_str()); }
	
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