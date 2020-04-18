#include "stdafx.h"
#include "ParserStageOutputSerializer.h"
#include "CompilerParts/StageOutputStructs.h"

ParserStageOutputSerializer::ParserStageOutputSerializer()
{
	m_docToSave.SetObject();

	m_allocator = &m_docToSave.GetAllocator();
}

void ParserStageOutputSerializer::Notify(const AbstractTreeNode* root)
{
	WValue tree = SerializeNode(root);
	
	m_docToSave.AddMember(L"Tree", tree, *m_allocator);
}

void ParserStageOutputSerializer::OpenDocToSave(const std::wstring& path)
{
	m_resFile.open(path + L".parser.json");
}

void ParserStageOutputSerializer::Finalize()
{
	WStringBuffer strbuf;
	rapidjson::Writer<WStringBuffer, rapidjson::UTF16<>> writer(strbuf);
	m_docToSave.Accept(writer);

	m_resFile.write(strbuf.GetString(), strbuf.GetLength());
}

WValue ParserStageOutputSerializer::SerializeNode(const AbstractTreeNode* node) const
{
	WValue res(rapidjson::kObjectType);

	WValue val(node->data.c_str(), node->data.length());
	
	res.AddMember(L"Value", val, *m_allocator);
	
	WValue arr(rapidjson::kArrayType);
	for (const AbstractTreeNode& child : node->m_childs)
		arr.PushBack(SerializeNode(&child), *m_allocator);

	res.AddMember(L"Nodes", arr, *m_allocator);
	
	return res;
}