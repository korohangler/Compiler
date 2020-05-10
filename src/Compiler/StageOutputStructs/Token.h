#pragma once

struct __declspec(dllexport) Token
{
	Token() : Line(0) {};
	Token(std::wstring val, std::wstring type, size_t line) : Value(std::move(val)), Type(std::move(type)), Line(line) {};

	std::wstring Value;
	std::wstring Type;
	size_t Line;

	bool operator==(const Token& tok) const { return tok.Line == Line && tok.Type == Type && tok.Value == Value; }
};
