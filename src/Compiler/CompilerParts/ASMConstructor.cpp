#include "stdafx.h"
#include "ASMConstructor.h"

struct Atom
{
	int data;
	int type;
};

const int atomSize = sizeof(Atom);

ASMConstructor::ASMConstructor(const std::wstring_view runningDirectory)
{
	m_configDirectory = std::wstring(runningDirectory) + L"../../../data/ASM/";

	for (std::filesystem::directory_iterator iter(m_configDirectory + L"Functions/"); !iter._At_end(); ++iter)
	{
		std::filesystem::path path = *iter;
		m_builtInFunctions.emplace_back(Utils::ReadFileAsString(path.wstring()));
	}
}

std::wstring ASMConstructor::GenerateASM()
{
	std::wstring result = Utils::ReadFileAsString(m_configDirectory + L"Header.txt");

	result += L"\n.data\n";
	std::for_each(begin(m_literals), end(m_literals), [&](const auto& elem)
	{
		if (elem.second->GetLiteralType() == Literal::LiteralType::String)
		{
			std::wstring data(elem.second->GetData().data());

			size_t index = 0;
			while (true) {
				index = data.find(L"'", index);
				if (index == std::string::npos) break;

				data.replace(index, 1, L"\'\'");

				index += 2;
			}

			result += std::wstring(elem.second->GetAttribute(L"ASMName").data()) + L" DB '" + data + L"', 00H\n";
		}
		else
		{
			float val = elem.second->GetFloatData();
			std::wstringstream hexRepresentation;
			hexRepresentation << std::hex << *((unsigned int*)(&val));
			result += std::wstring(elem.second->GetAttribute(L"ASMName").data()) + L" DD " + hexRepresentation.str() + L"r\n";
		}
	});

	result += L"\n.code\n";

	std::for_each(begin(m_builtInFunctions), end(m_builtInFunctions), [&](const auto& elem) { result += elem + L"\n"; });

	std::for_each(begin(m_userFunctions), end(m_userFunctions), [&](const auto& func)
	{
		result += func.Name + L" PROC C\n";
		result += L"push ebp\n";
		result += L"mov ebp, esp\n";

		std::for_each(begin(func.Commands), end(func.Commands), [&](const auto& elem)
		{
			result += elem + L"\n";
		});

		result += L"mov eax, 0\n";
		result += L"pop ebp\n";
		result += L"ret\n";
		result += func.Name + L" ENDP\n";
	});

	result += L"main PROC C\n";
	result += L"push ebp\n";
	result += L"mov ebp, esp\n";
	result += std::wstring(L"sub esp, ") + std::to_wstring((m_variables.size() + m_literals.size()) * atomSize) + L"\n";

	size_t stackSize = (m_variables.size() + m_literals.size()) * atomSize;

	for (size_t i = 0; i < stackSize / 4; i++)
	{
		if (!std::any_of(begin(m_literals), end(m_literals), [i](const auto& el) {
			const size_t pos = std::stoi(el.second->GetAttribute(L"PositionOnStack").data());
			return pos * atomSize == i * 4 || (pos  * atomSize) + 4 == i * 4;
			}))
		{
			result += std::wstring(L"mov DWORD PTR[ebp - ") + std::to_wstring(i * 4) + L"], 0\n";
		}
	}

	std::for_each(begin(m_literals), end(m_literals), [&](const auto& elem)
	{
		int posOnStack = std::stoi(elem.second->GetAttribute(L"PositionOnStack").data()) * atomSize;
		if (elem.second->GetLiteralType() == Literal::LiteralType::String)
		{
			result += std::wstring(L"mov DWORD PTR[ebp - ") + std::to_wstring(posOnStack) + L"], OFFSET " + elem.second->GetAttribute(L"ASMName").data() + L"\n";
			result += std::wstring(L"mov DWORD PTR[ebp - ") + std::to_wstring(posOnStack + 4) + L"], 1\n";
		}
		else
		{
			result += std::wstring(L"movss xmm0, DWORD PTR ") + elem.second->GetAttribute(L"ASMName").data() + L"\n";
			result += std::wstring(L"movss DWORD PTR[ebp - ") + std::to_wstring(posOnStack) + L"], xmm0\n";

			result += std::wstring(L"mov DWORD PTR[ebp - ") + std::to_wstring(posOnStack + 4) + L"], 0\n";
		}
	});

	std::for_each(begin(m_commands), end(m_commands), [&](const auto& elem) { result += elem + L"\n"; });

	result += std::wstring(L"add esp, ") + std::to_wstring(stackSize) + L'\n';
	result += L"mov eax, 0\n";
	result += L"pop ebp\n";
	result += L"ret\n";
	result += L"main ENDP\n";
	result += L"end\n";
	
	return result;
}

void ASMConstructor::PushArgsToStack(int a, int b, bool useESP)
{
	std::wstring reg = useESP ? L"esp" : L"ebp";

	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	commands.push_back(std::wstring(L"push [") + reg + L" - " + std::to_wstring(b * atomSize) + L"]");
	commands.push_back(std::wstring(L"push [") + reg + L" - " + std::to_wstring(b * atomSize + 4) + L"]");
	commands.push_back(std::wstring(L"push [") + reg + L" - " + std::to_wstring(a * atomSize) + L"]");
	commands.push_back(std::wstring(L"push [") + reg + L" - " + std::to_wstring(a * atomSize + 4) + L"]");
}

void ASMConstructor::PopArgsFromStack()
{
	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	commands.emplace_back(L"add esp, 20");
}

void ASMConstructor::pushToStack(int what, bool useESP)
{
	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	commands.push_back(std::wstring(L"push [ebp - ") + std::to_wstring(what * atomSize + 4) + L"]");
	commands.push_back(std::wstring(L"push [ebp - ") + std::to_wstring(what * atomSize) + L"]");

	int& stackOffset = m_pendingFunctions.empty() ? m_stackOffset : m_pendingFunctions.top().StackOffset;

	stackOffset++;
}

void ASMConstructor::pushValueToStack(int what)
{
	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	commands.push_back(std::wstring(L"push ") + std::to_wstring(what));

	int& stackOffset = m_pendingFunctions.empty() ? m_stackOffset : m_pendingFunctions.top().StackOffset;

	stackOffset++;
}

void ASMConstructor::popFromStack(int to, bool useESP)
{
	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	commands.push_back(std::wstring(L"pop [ebp - ") + std::to_wstring(to * atomSize) + L"]");
	commands.push_back(std::wstring(L"pop [ebp - ") + std::to_wstring(to * atomSize + 4) + L"]");

	int& stackOffset = m_pendingFunctions.empty() ? m_stackOffset : m_pendingFunctions.top().StackOffset;

	stackOffset--;
}

void ASMConstructor::popFromStack()
{
	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	commands.push_back(L"add esp, 8");

	int& stackOffset = m_pendingFunctions.empty() ? m_stackOffset : m_pendingFunctions.top().StackOffset;

	stackOffset--;
}

void ASMConstructor::doOperation(int to, int what, int whereToStore, bool useESP)
{
	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	// function
	commands.push_back(L"call DoOperation");
	PopArgsFromStack();

	// mov result to store location
	commands.push_back(std::wstring(L"mov edx, DWORD PTR[ebp - ") + std::to_wstring(whereToStore * atomSize + 4) + L"]");
	commands.push_back(std::wstring(L"mov eax, DWORD PTR[ebp - ") + std::to_wstring(whereToStore * atomSize) + L"]");
}

void ASMConstructor::doOperationPush(int to, int what, bool useESP)
{
	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	// call summ function
	commands.push_back(L"call DoOperation");
	PopArgsFromStack();

	// push result on stack
	commands.push_back(L"push edx");
	commands.push_back(L"push eax");
}

void ASMConstructor::BeginFuction(const std::wstring_view name)
{
	UserFunction func;
	func.Name = std::wstring(name);
	m_pendingFunctions.emplace(std::move(func));
}

void ASMConstructor::EndFuction()
{
	m_userFunctions.emplace_back(std::move(m_pendingFunctions.top()));
	m_pendingFunctions.pop();
}

void ASMConstructor::jumpIf(std::wstring_view mark)
{
	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	commands.emplace_back(L"pop ecx");
	commands.emplace_back(L"add esp, 4");
	commands.emplace_back(L"cmp ecx, 0");
	commands.emplace_back(std::wstring(L"je ") + mark.data());
}

void ASMConstructor::jump(std::wstring_view mark)
{
	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	commands.emplace_back(std::wstring(L"jmp ") + mark.data());
}

void ASMConstructor::addJumpMark(std::wstring_view mark)
{
	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	commands.emplace_back(std::wstring(mark.data()) + L":");
}

std::wstring_view ASMConstructor::GetLiterPosition(std::wstring_view liter)
{
	return m_literals[liter.data()]->GetAttribute(L"PositionOnStack");
}

void ASMConstructor::moveAtom(int what, int to, bool leftUseESP, bool rightUseESP)
{
	std::wstring leftReg  = leftUseESP ? L"esp" : L"ebp";
	std::wstring rightReg = rightUseESP ? L"esp" : L"ebp";

	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	commands.push_back(std::wstring(L"mov [") + leftReg + L" - " + std::to_wstring(to * atomSize) + L"], "	 + L"[" + rightReg + L" - " + std::to_wstring(to * atomSize) + L"]");
	commands.push_back(std::wstring(L"mov [") + leftReg + L" - " + std::to_wstring(to * atomSize + 4) + L"], " + L"[" + rightReg + L" - " + std::to_wstring(to * atomSize) + L"]");
}

void ASMConstructor::copyAtom(int what, int to, bool leftUseESP, bool rightUseESP)
{
	std::wstring leftReg = leftUseESP ? L"esp" : L"ebp";
	std::wstring rightReg = rightUseESP ? L"esp" : L"ebp";

	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	pushToStack(what, leftUseESP);
	commands.push_back(L"call CopyAtom");

	// move result to asked location
	commands.push_back(std::wstring(L"mov [") + rightReg + L" - " + std::to_wstring(to * atomSize)     + L"], eax");
	commands.push_back(std::wstring(L"mov [") + rightReg + L" - " + std::to_wstring(to * atomSize + 4) + L"], edx");

	// pop args
	commands.push_back(L"add esp, 8");
}

void ASMConstructor::call(const std::wstring_view name, int whereToStore, bool useESP, bool isVoid)
{
	const std::wstring reg = useESP ? L"esp" : L"ebp";

	std::vector<std::wstring>& commands = m_pendingFunctions.empty() ? m_commands : m_pendingFunctions.top().Commands;

	commands.emplace_back(std::wstring(L"call ") + name.data());

	if (!isVoid)
	{
		commands.push_back(std::wstring(L"mov [") + reg + L" - " + std::to_wstring(whereToStore * atomSize) + L"], eax");
		commands.push_back(std::wstring(L"mov [") + reg + L" - " + std::to_wstring(whereToStore * atomSize + 4) + L"], edx");
	}
}

void ASMConstructor::AddLiteral(const std::shared_ptr<Literal> val)
{
	if (m_literals.find(val->GetData().data()) == m_literals.end())
		m_literals.emplace(val->GetData().data(), val);
}
