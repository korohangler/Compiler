#pragma once

struct __declspec(dllexport) ScopeInfo
{
	std::wstring ParentScope;
	
	std::unordered_set<std::wstring> Identificators;
};

class __declspec(dllexport) IdentificatorTable
{
public:
	IdentificatorTable() = default;

	void AddScope(const std::wstring& parentScope, const std::wstring& scope);

	void AddIdentificator(const std::wstring& scope, const std::wstring& identificator);

	[[nodiscard]] bool IsIdentificatorExist(const std::wstring& scope, const std::wstring& variable);

	const std::unordered_map<std::wstring, ScopeInfo>& GetScopesMap() { return m_scopes; }
	
private:

	std::unordered_map<std::wstring, ScopeInfo> m_scopes;
};
