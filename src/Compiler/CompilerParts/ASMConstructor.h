#pragma once

class ASMConstructor
{
public:
	ASMConstructor(const std::wstring& runningDirectory);

	void pushToStack(int what, bool useESP = false);
	void popFromStack(int to, bool useESP = false);

	void doOperation(int to, int what, int whereToStore, bool useESP = false);
	void doOperationPush(int to, int what, bool useESP = false);

	void moveAtom(int what, int to, bool leftUseESP = false, bool rightUseESP = false);

	void copyAtom(int what, int to, bool leftUseESP = false, bool rightUseESP = false);

	void AddVariable(std::wstring var);
	
	size_t GetLiteralPos(const std::wstring& val);

	int GetCurrentStackOffset() { return m_stackOffset; };

	std::wstring GenerateASM();
private:
	void PushArgsToStack(int a, int b, bool useESP);
	void PopArgsFromStack();

	std::vector<std::pair<std::wstring, float>> m_variables;
	std::map<std::wstring, size_t> m_literals;
	std::vector<std::wstring> m_commands;

	size_t m_literalsCounter = 0;

	std::vector<std::wstring> m_builtInFunctions;

	std::wstring m_configDirectory;

	int m_stackOffset = 0;
};