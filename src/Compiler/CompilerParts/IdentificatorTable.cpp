#include "stdafx.h"
#include "IdentificatorTable.h"

void IdentificatorTable::AddScope(const std::wstring& parentScope, const std::wstring& scope)
{
	ScopeInfo newScope;
	newScope.ParentScope = parentScope;

	m_scopes.emplace(scope, newScope);
}

void IdentificatorTable::AddIdentificator(const std::wstring& scope, const std::wstring& identificator)
{
	auto scopeToInsert = m_scopes.find(scope);

	auto& setToInsert = scopeToInsert->second.Identificators;

	Utils::ASSERT2(setToInsert.count(identificator) == 0, std::wstring(L"Identificator: ") + identificator + L". Already exists!");
	
	setToInsert.insert(identificator);
}

bool IdentificatorTable::IsIdentificatorExist(const std::wstring& scope, const std::wstring& variable)
{
	auto treeScope = m_scopes.find(scope);
	if (treeScope == m_scopes.end()) return false;

	if (treeScope->second.Identificators.find(variable) != treeScope->second.Identificators.end())
		return true;

	return IsIdentificatorExist(treeScope->second.ParentScope, variable);
}
