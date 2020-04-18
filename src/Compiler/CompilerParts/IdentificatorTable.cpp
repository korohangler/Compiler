#include "stdafx.h"
#include "IdentificatorTable.h"

IdentificatorTable* IdentificatorTable::m_instance = nullptr;

void IdentificatorTable::AddScope(const std::string& parentScope, const std::string& scope)
{
	ScopeInfo newScope;
	newScope.ParentScope = parentScope;

	m_scopes.emplace(scope, newScope);
}

void IdentificatorTable::AddIdentificator(const std::string& scope, const std::string& identificator)
{
	m_scopes.find(scope)->second.Identificators.insert(identificator);
}

bool IdentificatorTable::IsVariableExist(std::string scope, std::string variable)
{
	auto treeScope = m_scopes.find(scope);
	if (treeScope == m_scopes.end()) return false;

	if (treeScope->second.Identificators.find(variable) != treeScope->second.Identificators.end())
		return true;

	return IsVariableExist(treeScope->second.ParentScope, variable);
}
