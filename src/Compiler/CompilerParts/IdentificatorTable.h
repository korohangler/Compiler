#pragma once

struct ScopeInfo
{
	std::wstring ParentScope;
	
	std::unordered_set<std::wstring> Identificators;
};

class IdentificatorTable
{
public:
	IdentificatorTable(const IdentificatorTable& table) = delete;
	IdentificatorTable(IdentificatorTable&& table) = delete;
	IdentificatorTable operator=(const IdentificatorTable& a) = delete;
	IdentificatorTable& operator=(IdentificatorTable&& a) = delete;
	~IdentificatorTable() = default;
	
	static IdentificatorTable& GetInstance() { return m_instance; }

	void AddScope(const std::wstring& parentScope, const std::wstring& scope);

	void AddIdentificator(const std::wstring& scope, const std::wstring& identificator);

	bool IsVariableExist(const std::wstring& scope, const std::wstring& variable);

private:
	IdentificatorTable() = default;
	
	static IdentificatorTable m_instance;

	std::unordered_map<std::wstring, ScopeInfo> m_scopes;
};
