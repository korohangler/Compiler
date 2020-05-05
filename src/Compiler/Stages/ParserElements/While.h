#pragma once
#include "CompilerParts/StageOutputStructs.h"

class While : public AbstractTreeNode
{
public:
	~While() override = default;
	
	[[nodiscard]] bool IsComplete() const override { return m_isCompleted; }

	void Compute(const Token& token) override;

	[[nodiscard]] bool NeedRecompute() const override { return m_needRecompute; }

	const std::wstring& GetScopeName() override { return parent->GetScopeName(); }

	void SetScopeName(const std::wstring& name) override { }

	[[nodiscard]] const std::wstring& GetSerializeData() const override { return m_serializeData; }

private:

	std::wstring m_functionName;

	std::wstring m_serializeData;

	bool m_isCompleted = false;

	bool m_needRecompute = false;

	size_t m_counter = 0;
};