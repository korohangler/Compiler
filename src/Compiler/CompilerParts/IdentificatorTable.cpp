#include "stdafx.h"
#include "IdentificatorTable.h"

void IdentificatorTable::AddScope(const std::wstring_view parentScope, const std::wstring_view scope)
{
	ScopeInfo newScope;
	newScope.ParentScope = parentScope;

	m_scopes.emplace(scope, newScope);
}

void IdentificatorTable::AddIdentificator(const std::wstring_view scope, const std::wstring_view name, IdentificatorInfo identificator)
{
	auto scopeToInsert = m_scopes.find(scope.data());

	auto& setToInsert = scopeToInsert->second.Identificators;

	Utils::ASSERT2(setToInsert.count(name.data()) == 0, std::wstring(L"Identificator: ") + name.data() + L". Already exists!");
	
	setToInsert.emplace(name, std::move(identificator));
}

bool IdentificatorTable::IsIdentificatorExist(const std::wstring_view scope, const std::wstring_view variable)
{
	auto treeScope = m_scopes.find(scope.data());
	if (treeScope == m_scopes.end()) return false;

	if (treeScope->second.Identificators.find(variable.data()) != treeScope->second.Identificators.end())
		return true;

	return IsIdentificatorExist(treeScope->second.ParentScope, variable);
}

IdentificatorInfo& IdentificatorTable::GetIdentificatorInfo(const std::wstring_view scope, const std::wstring_view name)
{
	auto treeScope = m_scopes.find(scope.data());

	if (treeScope->second.Identificators.find(name.data()) != treeScope->second.Identificators.end())
		return treeScope->second.Identificators[name.data()];

	return GetIdentificatorInfo(treeScope->second.ParentScope, name);
}
