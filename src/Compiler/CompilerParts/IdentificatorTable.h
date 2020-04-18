#pragma once

struct ScopeInfo
{
	std::string ParentScope;
	
	std::unordered_set<std::string> Identificators;
};

class IdentificatorTable
{
public:
	static IdentificatorTable& GetInstance() { if (m_instance == nullptr) m_instance = new IdentificatorTable(); return *m_instance; }

	void AddScope(const std::string& parentScope, const std::string& scope);

	void AddIdentificator(const std::string& scope, const std::string& identificator);

	bool IsVariableExist(std::string scope, std::string variable);

private:
	IdentificatorTable();
	IdentificatorTable(const IdentificatorTable& table);

	static IdentificatorTable* m_instance;

	std::unordered_map<std::string, ScopeInfo> m_scopes;
};
