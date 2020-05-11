#pragma once

struct __declspec(dllexport) IdentificatorInfo
{
	std::wstring AsmName;
	size_t VariableLocationOnStack; // as idx
};

struct __declspec(dllexport) ScopeInfo
{
	std::wstring ParentScope;
	
	std::unordered_map<std::wstring, IdentificatorInfo> Identificators;
};

class __declspec(dllexport) IdentificatorTable
{
public:
	IdentificatorTable() = default;

	void AddScope(const std::wstring& parentScope, const std::wstring& scope);

	void AddIdentificator(const std::wstring& scope, const std::wstring& name, IdentificatorInfo identificator);

	[[nodiscard]] bool IsIdentificatorExist(const std::wstring& scope, const std::wstring& name);

	IdentificatorInfo& GetIdentificatorInfo(const std::wstring& scope, const std::wstring& name);
	
	const std::unordered_map<std::wstring, ScopeInfo>& GetScopesMap() const { return m_scopes; }

private:

	std::unordered_map<std::wstring, ScopeInfo> m_scopes;
};
