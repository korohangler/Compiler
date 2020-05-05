#pragma once

class ParserStage;
class IStageOutputSerializer;

struct __declspec(dllexport) Token
{
	Token() : Line(0) {};
	Token(std::wstring val, std::wstring type, size_t line) : Value(std::move(val)), Type(std::move(type)), Line(line) {};

	std::wstring Value;
	std::wstring Type;
	size_t Line;

	bool operator==(const Token& tok) const { return tok.Line == Line && tok.Type == Type && tok.Value == Value; }
};

class __declspec(dllexport) AbstractTreeNode
{
public:
	virtual ~AbstractTreeNode() = default;
	AbstractTreeNode() = default;
	AbstractTreeNode(size_t childsCount) { m_childs = std::vector<std::shared_ptr<AbstractTreeNode>>(childsCount); }
	
	AbstractTreeNode* parent = nullptr;
	std::vector<std::shared_ptr<AbstractTreeNode>> m_childs;

	[[nodiscard]] virtual bool IsComplete() const = 0;

	virtual void Compute(const Token& token) = 0;

	[[nodiscard]] virtual bool NeedRecompute() const = 0;

	[[nodiscard]] std::wstring GetTypeName() const { return Utils::StringConverter::StrToWstr(typeid(*this).name()); }

	[[nodiscard]] virtual const std::wstring& GetSerializeData() const = 0;
};
