#pragma once

struct AbstractTreeNode
{
	AbstractTreeNode* parent = nullptr;

	std::wstring type;
	std::wstring data;

	std::vector<AbstractTreeNode> m_childs;
};

struct Token
{
	Token() : Line(0) {};
	Token(std::wstring val, std::wstring type, size_t line) : Value(val), Type(type), Line(line) {};

	std::wstring Value;
	std::wstring Type;
	size_t Line;

	bool operator==(const Token& tok) const { return tok.Line == Line && tok.Type == Type && tok.Value == Value; }
};