#pragma once

class ASMConstructor
{
public:
	ASMConstructor(const std::wstring& runningDirectory);

	void addPush(int to, int what, bool useESP = false);			   // summ 2 atoms and push result to stack
	void add(int to, int what, int whereToStore, bool useESP = false); // summ 2 atoms and store it

	void subPush(int to, int what, bool useESP = false);			   // sub 2 atoms and push result to stack
	void sub(int to, int what, int whereToStore, bool useESP = false); // sub 2 atoms and store it

	void divPush(int to, int what, bool useESP = false);			   // div 2 atoms and push result to stack
	void div(int to, int what, int whereToStore, bool useESP = false); // div 2 atoms and store it

	void mulPush(int to, int what, bool useESP = false);			   // mul 2 atoms and push result to stack
	void mul(int to, int what, int whereToStore, bool useESP = false); // mul 2 atoms and store it

	void pushToStack(int what, bool useESP = false);
	void popFromStack(int to, bool useESP = false);

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