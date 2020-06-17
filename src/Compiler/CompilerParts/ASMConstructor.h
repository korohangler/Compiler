#pragma once
#include "Stages/ParserElements/Literal.h"
#include "Stages/ParserElements/Identificator.h"

class ASMConstructor
{
public:
	ASMConstructor(const std::wstring_view runningDirectory);

	void pushToStack(int what, bool useESP = false);
	void pushValueToStack(int what);
	void popFromStack(int to, bool useESP = false);
	void popFromStack();

	void doOperation(int to, int what, int whereToStore, bool useESP = false);
	void doOperationPush(int to, int what, bool useESP = false);

	void BeginFuction(const std::wstring_view name);
	void EndFuction();

	void moveAtom(int what, int to, bool leftUseESP = false, bool rightUseESP = false);

	void copyAtom(int what, int to, bool leftUseESP = false, bool rightUseESP = false);

	void call(const std::wstring_view name, int whereToStore, bool useESP = false, bool isVoid = false);

	void AddVariable(const std::shared_ptr<Identificator> var) { m_variables.emplace_back(var); }
	
	void AddLiteral(const std::shared_ptr<Literal> val);

	int GetCurrentStackOffset() { return m_pendingFunctions.empty() ? m_stackOffset : m_pendingFunctions.top().StackOffset; };

	std::wstring GenerateASM();
private:
	void PushArgsToStack(int a, int b, bool useESP);
	void PopArgsFromStack();

	std::vector<std::shared_ptr<Identificator>> m_variables;
	std::map<std::wstring, std::shared_ptr<Literal>> m_literals;
	std::vector<std::wstring> m_commands;

	int m_literalsCounter = 0;

	std::vector<std::wstring> m_builtInFunctions;

	struct UserFunction
	{
		std::wstring Name;
		std::vector<std::wstring> Commands;
		int StackOffset = 0;
	};

	std::stack<UserFunction> m_pendingFunctions;

	std::vector<UserFunction> m_userFunctions;

	std::wstring m_configDirectory;

	int m_stackOffset = 0;
};