#include "stdafx.h"
#include "ASMConstructor.h"

void ASMConstructor::mov(const std::wstring& to, const std::wstring& what)
{
	m_commands.emplace_back(L"mov " + to + L", " + what);
}

void ASMConstructor::add(const std::wstring& to, const std::wstring& what)
{
	m_commands.emplace_back(L"add " + to + L", " + what);
}

void ASMConstructor::fld(const std::wstring& what)
{
	m_commands.emplace_back(L"fld " + what);
}

void ASMConstructor::faddp()
{
	m_commands.emplace_back(L"faddp");
}

void ASMConstructor::fst(const std::wstring& to)
{
	m_commands.emplace_back(L"fst " + to);
}

void ASMConstructor::fstp(const std::wstring& to)
{
	m_commands.emplace_back(L"fstp " + to);
}

void ASMConstructor::jmp(const std::wstring& to)
{
	m_commands.emplace_back(L"jmp " + to);
}

void ASMConstructor::push(const std::wstring& what)
{
	m_commands.emplace_back(L"psh " + what);
}

void ASMConstructor::pop(const std::wstring& to)
{
	m_commands.emplace_back(L"pop " + to);
}

void ASMConstructor::AddVariable(std::wstring var, float defaultVal)
{
	m_variables.emplace_back(var, defaultVal);
}

const std::wstring& ASMConstructor::GetConstant(float val)
{
	if (m_constants.count(val) == 0)
	{
		m_constants[val] = L"constant" + std::to_wstring(m_counter);

		m_counter++;
	}
	
	return m_constants[val];
}

std::wstring ASMConstructor::GenerateASM()
{
	std::wstring result = L".586\n";
	result += L".model flat, stdcall\n";
	result += L".stack 4096\n";
	result += L"option casemap:none\n";
	result += L"includelib libcmt.lib\n";
	result += L"includelib libvcruntime.lib\n";
	result += L"includelib libucrt.lib\n";
	result += L"includelib legacy_stdio_definitions.lib\n";
	result += L"ExitProcess PROTO, dwExitCode:DWORD\n";

	result += L".const\n";
	for(auto it : m_constants)
		result += it.second + L" REAL4 " + std::to_wstring(it.first) + L"\n";

	result += L".data\n";
	for (auto it : m_variables)
		result += it.first + L" REAL4 " + std::to_wstring(it.second) + L"\n";

	result += L".code\n";
	result += L"main PROC C\n";

	for (auto it : m_commands)
		result += it + L"\n";

	result += L"mov eax, 0\n";
	result += L"ret\n";
	result += L"main ENDP\n";
	result += L"end\n";
	
	return result;
}
