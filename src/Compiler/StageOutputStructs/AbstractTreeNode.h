#pragma once
#include "Token.h"

class __declspec(dllexport) AbstractTreeNode
{
public:
	virtual ~AbstractTreeNode() = default;
	AbstractTreeNode() = default;
	AbstractTreeNode(size_t childsCount) : Childs(childsCount) { }

	AbstractTreeNode* Parent = nullptr;
	std::vector<std::shared_ptr<AbstractTreeNode>> Childs;

	[[nodiscard]] virtual bool IsComplete() const = 0;

	virtual void Compute(const Token& token) = 0;

	[[nodiscard]] virtual bool NeedRecompute() const = 0;

	[[nodiscard]] std::wstring GetTypeName() const { return Utils::StringConverter::StrToWstr(typeid(*this).name()); }

	[[nodiscard]] virtual const std::wstring& GetSerializeData() const = 0;

	void SetAttribute(const std::wstring& attrName, const std::wstring& attr) { m_attributes[attrName] = attr; }

	[[nodiscard]] const std::wstring& GetAttribute(const std::wstring& attrName) { return m_attributes[attrName]; }

private:
	std::unordered_map<std::wstring, std::wstring> m_attributes;
};
