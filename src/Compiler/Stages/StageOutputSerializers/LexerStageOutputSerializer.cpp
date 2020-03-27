#include "stdafx.h"
#include "LexerStageOutputSerializer.h"

LexerStageOutputSerializer::LexerStageOutputSerializer()
{
	m_docToSave.SetObject();

	m_allocator = &m_docToSave.GetAllocator();
	m_arrayToSave = WValue(rapidjson::kArrayType);
}

void LexerStageOutputSerializer::Notify(LexerStage::Token token)
{
	WValue objValue;
	objValue.SetObject();

	WValue tmp;
	tmp.SetString(WValue::StringRefType(token.Type.c_str()), *m_allocator);
	objValue.AddMember(L"Type", tmp, *m_allocator);

	tmp.SetString(WValue::StringRefType(token.Value.c_str()), *m_allocator);
	objValue.AddMember(L"Value", tmp, *m_allocator);

	std::wstring line = std::to_wstring(token.Line);
	tmp.SetString(WValue::StringRefType(line.c_str()), *m_allocator);
	objValue.AddMember(L"Line", tmp, *m_allocator);

	m_arrayToSave.PushBack(objValue, *m_allocator);
}

void LexerStageOutputSerializer::OpenDocToSave(const std::wstring& path)
{
	m_resFile.open(path + L".lexer");
}

void LexerStageOutputSerializer::Finalize()
{
	m_docToSave.AddMember(L"Tokens", m_arrayToSave, *m_allocator);
	WStringBuffer strbuf;
	rapidjson::Writer<WStringBuffer, rapidjson::UTF16<>> writer(strbuf);
	m_docToSave.Accept(writer);

	m_resFile.write(strbuf.GetString(), strbuf.GetLength());
}
