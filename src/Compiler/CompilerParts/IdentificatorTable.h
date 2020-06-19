#pragma once

struct __declspec(dllexport) IdentificatorInfo
{
	std::wstring AsmName;
	int VariableLocationOnStack; // as idx
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

	void AddScope(const std::wstring_view parentScope, const std::wstring_view scope);

	void AddIdentificator(const std::wstring_view scope, const std::wstring_view name, IdentificatorInfo identificator);

	[[nodiscard]] bool IsIdentificatorExist(const std::wstring_view scope, const std::wstring_view name);

	IdentificatorInfo& GetIdentificatorInfo(const std::wstring_view scope, const std::wstring_view name);
	
	const std::unordered_map<std::wstring, ScopeInfo>& GetScopesMap() const { return m_scopes; }

	static const inline std::vector<std::wstring> UserAvailableFunctions
	{
		L"print",
		L"ScanFloat",
		L"ScanString"
	};

private:

	std::unordered_map<std::wstring, ScopeInfo> m_scopes;
};
