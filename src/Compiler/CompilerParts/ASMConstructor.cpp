#include "stdafx.h"
#include "ASMConstructor.h"

struct Atom
{
	int data;
	int type;
};

const size_t atomSize = sizeof(Atom);

void ASMConstructor::AddVariable(std::wstring var)
{
	m_variables.emplace_back(var, 0.0);
}

size_t ASMConstructor::GetLiteralPos(const std::wstring& val)
{
	auto it = m_literals.find(val);

	if (it == m_literals.end())
	{
		m_literals[val] = m_literalsCounter;
		m_literalsCounter++;
	}

	return m_literals[val];
}

std::wstring ASMConstructor::GenerateASM()
{
	std::wstring result = Utils::ReadFileAsString(m_configDirectory + L"Header.txt");

	result += L"\n.code\n";

	for (auto it : m_builtInFunctions)
	{
		result += it;
		result += L"\n";
	}

	result += L"main PROC C\n";
	result += L"push ebp\n";
	result += L"mov ebp, esp\n";
	result += std::wstring(L"sub esp, ") + std::to_wstring((m_variables.size() + m_literals.size()) * atomSize) + L"\n";

	int stackSize = (m_variables.size() + m_literals.size()) * atomSize;

	for (int i = 0; i < stackSize / 4; i++)
		result += std::wstring(L"mov DWORD PTR[ebp - ") + std::to_wstring(i * 4) + L"], 0\n";

	for (auto it : m_commands)
		result += it + L"\n";

	result += L"mov eax, 0\n";
	result += L"ret\n";
	result += L"main ENDP\n";
	result += L"end\n";
	
	return result;
}

void ASMConstructor::PushArgsToStack(int a, int b, bool useESP)
{
	std::wstring reg = useESP ? L"esp" : L"ebp";

	m_commands.push_back(std::wstring(L"push [") + reg + L" - " + std::to_wstring(b * atomSize) + L"]");
	m_commands.push_back(std::wstring(L"push [") + reg + L" - " + std::to_wstring(b * atomSize + 4) + L"]");
	m_commands.push_back(std::wstring(L"push [") + reg + L" - " + std::to_wstring(a * atomSize) + L"]");
	m_commands.push_back(std::wstring(L"push [") + reg + L" - " + std::to_wstring(a * atomSize + 4) + L"]");
}

void ASMConstructor::PopArgsFromStack()
{
	m_commands.push_back(L"pop ecx");
	m_commands.push_back(L"pop ecx");
	m_commands.push_back(L"pop ecx");
	m_commands.push_back(L"pop ecx");
}

ASMConstructor::ASMConstructor(const std::wstring& runningDirectory)
{
	m_configDirectory = runningDirectory + L"../../../data/ASM/";

	for (std::filesystem::directory_iterator iter(m_configDirectory + L"Functions/"); !iter._At_end(); ++iter)
	{
		std::filesystem::path path = *iter;
		m_builtInFunctions.emplace_back(Utils::ReadFileAsString(path.wstring()));
	}
}

void ASMConstructor::addPush(int to, int what, bool useESP)
{
	// call summ function
	m_commands.push_back(L"call summAtoms");
	PopArgsFromStack();

	// push result on stack
	m_commands.push_back(L"push eax");
	m_commands.push_back(L"push ebx");
}

void ASMConstructor::add(int to, int what, int whereToStore, bool useESP)
{
	std::wstring reg = useESP ? L"esp" : L"ebp";

	// call summ function
	m_commands.push_back(L"call summAtoms");
	PopArgsFromStack();

	// mov result to store location
	m_commands.push_back(std::wstring(L"mov eax, DWORD PTR[") + reg + L" - " + std::to_wstring(whereToStore * atomSize) + L"]");
	m_commands.push_back(std::wstring(L"mov ebx, DWORD PTR[") + reg + L" - " + std::to_wstring(whereToStore * atomSize + 4) + L"]");
}

void ASMConstructor::subPush(int to, int what, bool useESP)
{
	// call summ function
	m_commands.push_back(L"call subAtoms");
	PopArgsFromStack();

	// push result on stack
	m_commands.push_back(L"push eax");
	m_commands.push_back(L"push ebx");
}

void ASMConstructor::sub(int to, int what, int whereToStore, bool useESP)
{
	// call summ function
	m_commands.push_back(L"call subAtoms");
	PopArgsFromStack();

	// mov result to store location
	m_commands.push_back(std::wstring(L"mov eax, DWORD PTR[ebp - ") + std::to_wstring(whereToStore * atomSize) + L"]");
	m_commands.push_back(std::wstring(L"mov ebx, DWORD PTR[ebp - ") + std::to_wstring(whereToStore * atomSize + 4) + L"]");
}

void ASMConstructor::divPush(int to, int what, bool useESP)
{
	// call summ function
	m_commands.push_back(L"call divAtoms");
	PopArgsFromStack();

	// push result on stack
	m_commands.push_back(L"push eax");
	m_commands.push_back(L"push ebx");
}

void ASMConstructor::div(int to, int what, int whereToStore, bool useESP)
{
	// call summ function
	m_commands.push_back(L"call divAtoms");
	PopArgsFromStack();

	// mov result to store location
	m_commands.push_back(std::wstring(L"mov eax, DWORD PTR[ebp - ") + std::to_wstring(whereToStore * atomSize) + L"]");
	m_commands.push_back(std::wstring(L"mov ebx, DWORD PTR[ebp - ") + std::to_wstring(whereToStore * atomSize + 4) + L"]");
}

void ASMConstructor::mulPush(int to, int what, bool useESP)
{
	// call summ function
	m_commands.push_back(L"call mulAtoms");
	PopArgsFromStack();

	// push result on stack
	m_commands.push_back(L"push eax");
	m_commands.push_back(L"push ebx");
}

void ASMConstructor::mul(int to, int what, int whereToStore, bool useESP)
{
	// call summ function
	m_commands.push_back(L"call mulAtoms");
	PopArgsFromStack();

	// mov result to store location
	m_commands.push_back(std::wstring(L"mov eax, DWORD PTR[ebp - ") + std::to_wstring(whereToStore * atomSize) + L"]");
	m_commands.push_back(std::wstring(L"mov ebx, DWORD PTR[ebp - ") + std::to_wstring(whereToStore * atomSize + 4) + L"]");
}

void ASMConstructor::pushToStack(int what, bool useESP)
{
	m_commands.push_back(std::wstring(L"push [ebp - ") + std::to_wstring(what * atomSize) + L"]");
	m_commands.push_back(std::wstring(L"push [ebp - ") + std::to_wstring(what * atomSize + 4) + L"]");

	m_stackOffset++;
}

void ASMConstructor::popFromStack(int to, bool useESP)
{
	m_commands.push_back(std::wstring(L"pop [ebp - ") + std::to_wstring(to * atomSize) + L"]");
	m_commands.push_back(std::wstring(L"pop [ebp - ") + std::to_wstring(to * atomSize + 4) + L"]");

	m_stackOffset--;
}
