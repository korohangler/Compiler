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
				WValue idInfo;
				idInfo.SetObject();

				WValue val;
				val.SetString(WValue::StringRefType(id.first.c_str()), *m_allocator);
				idInfo.AddMember(L"Name", val, *m_allocator);

				val.SetString(WValue::StringRefType(id.second.AsmName.c_str()), *m_allocator);
				idInfo.AddMember(L"AsmName", val, *m_allocator);

				val.SetString(WValue::StringRefType(std::to_wstring(id.second.VariableLocationOnStack).c_str()), *m_allocator);
				idInfo.AddMember(L"LocationOnStack", val, *m_allocator);

				tmp.PushBack(idInfo, *m_allocator);
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

void IdentificatorTableSerializer::OpenDocToSave(const std::wstring_view path)
{
	m_resFile.open(std::wstring(path) + L".IDTable.json");
}
