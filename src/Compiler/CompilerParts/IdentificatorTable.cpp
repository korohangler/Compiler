#include "stdafx.h"
#include "IdentificatorTable.h"

IdentificatorTable IdentificatorTable::m_instance;

void IdentificatorTable::AddScope(const std::wstring& parentScope, const std::wstring& scope)
{
	ScopeInfo newScope;
	newScope.ParentScope = parentScope;

	m_scopes.emplace(scope, newScope);
}

void IdentificatorTable::AddIdentificator(const std::wstring& scope, const std::wstring& identificator)
{
	m_scopes.find(scope)->second.Identificators.insert(identificator);
}

bool IdentificatorTable::IsVariableExist(const std::wstring& scope, const std::wstring& variable)
{
	auto treeScope = m_scopes.find(scope);
	if (treeScope == m_scopes.end()) return false;

	if (treeScope->second.Identificators.find(variable) != treeScope->second.Identificators.end())
		return true;

	return IsVariableExist(treeScope->second.ParentScope, variable);
}
