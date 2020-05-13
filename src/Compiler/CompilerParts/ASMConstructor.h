#pragma once

class ASMConstructor
{
public:
	ASMConstructor(const std::wstring_view runningDirectory);

	void pushToStack(int what, bool useESP = false);
	void popFromStack(int to, bool useESP = false);

	void doOperation(int to, int what, int whereToStore, bool useESP = false);
	void doOperationPush(int to, int what, bool useESP = false);

	void moveAtom(int what, int to, bool leftUseESP = false, bool rightUseESP = false);

	void copyAtom(int what, int to, bool leftUseESP = false, bool rightUseESP = false);

	void AddVariable(const std::wstring_view var);
	
	int GetLiteralPos(const std::wstring_view val);

	int GetCurrentStackOffset() { return m_stackOffset; };

	std::wstring GenerateASM();
private:
	void PushArgsToStack(int a, int b, bool useESP);
	void PopArgsFromStack();

	std::vector<std::pair<std::wstring, float>> m_variables;
	std::map<std::wstring, int> m_literals;
	std::vector<std::wstring> m_commands;

	int m_literalsCounter = 0;

	std::vector<std::wstring> m_builtInFunctions;

	std::wstring m_configDirectory;

	int m_stackOffset = 0;
};