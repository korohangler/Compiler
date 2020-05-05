#pragma once
#include "IStageOutputSerializer.h"
#include "CompilerParts/StageOutputStructs.h"
#include "CompilerParts/Observers/IObserver.h"

class __declspec(dllexport) LexerStageOutputSerializer : public IStageOutputSerializer, public IObserver<const Token&>
{
public:
	LexerStageOutputSerializer();

	/// IObserver override
	void Notify(const Token& token) override;
	///

	/// IStageOutputSerializer override
	void OpenDocToSave(const std::wstring& path) override;

	void Finalize() override {};
	///

private:

	WDocument m_docToSave;
	WValue	  m_arrayToSave;

	WDocument::AllocatorType* m_allocator;

	std::wofstream m_resFile;
};