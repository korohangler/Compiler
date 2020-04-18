#pragma once

struct AbstractTreeNode
{
	AbstractTreeNode* parent = nullptr;

	std::wstring type;
	std::wstring data;

	std::vector<AbstractTreeNode> m_childs;
};