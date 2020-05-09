#pragma once

class ASMConstructor
{
public:
	void mov(const std::wstring& to, const std::wstring& what);
	void add(const std::wstring& to, const std::wstring& what);

	void fld(const std::wstring& what);
	void faddp();
	void fst(const std::wstring& to);
	void fstp(const std::wstring& to);

	void jmp(const std::wstring& to);

	void push(const std::wstring& what);
	void pop(const std::wstring& to);

	void AddVariable(std::wstring var, float defaultVal = 0.0);

	const std::wstring& GetConstant(float val);

	std::wstring GenerateASM();
private:
	std::unordered_map<float, std::wstring> m_constants;
	
	std::vector<std::pair<std::wstring, float>> m_variables;
	
	std::vector<std::wstring> m_commands;

	size_t m_counter = 0;
};