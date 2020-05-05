#include "stdafx.h"
#include "IdentificatorTableSerializer.h"

IdentificatorTableSerializer::IdentificatorTableSerializer()
{
	m_docToSave.SetObject();

	m_allocator = &m_docToSave.GetAllocator();
}

void IdentificatorTableSerializer::Notify(std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>> data)
{
	WValue arrayToSave(rapidjson::kArrayType);
	
	for(auto scope : data.second->GetScopesMap())
	{
		WValue objValue;
		objValue.SetObject();

		WValue tmp;
		
		tmp.SetString(WValue::StringRefType(scope.first.c_str()), *m_allocator);
		objValue.AddMember(L"Scope name", tmp, *m_allocator);

		tmp.SetString(WValue::StringRefType(scope.second.ParentScope.c_str()), *m_allocator);
		objValue.AddMember(L"Parent scope", tmp, *m_allocator);

		if (!scope.second.Identificators.empty())
		{
			tmp.SetArray();
			for (auto& id : scope.second.Identificators)
			{
				WValue idName;
				idName.SetString(WValue::StringRefType(id.c_str()), *m_allocator);
				tmp.PushBack(idName, *m_allocator);
			}
			objValue.AddMember(L"Identificators", tmp, *m_allocator);
		}

		arrayToSave.PushBack(objValue, *m_allocator);
	}

	m_docToSave.AddMember(L"Scopes", arrayToSave, *m_allocator);
	WStringBuffer strbuf;
	rapidjson::Writer<WStringBuffer, rapidjson::UTF16<>> writer(strbuf);
	m_docToSave.Accept(writer);

	m_resFile.write(strbuf.GetString(), strbuf.GetLength());
}

void IdentificatorTableSerializer::OpenDocToSave(const std::wstring& path)
{
	m_resFile.open(path + L".IDTable.json");
}
