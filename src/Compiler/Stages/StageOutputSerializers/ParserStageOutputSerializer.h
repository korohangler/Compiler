#pragma once
#include "CompilerParts/Observers/INewParserTreeObserver.h"
#include "IStageOutputSerializer.h"

class __declspec(dllexport) ParserStageOutputSerializer : public IStageOutputSerializer, public INewParserTreeObserver
{
public:
	ParserStageOutputSerializer();
	
	/// INewParserTreeObserver override
	void Notify(std::shared_ptr<AbstractTreeNode> root) override;
	///
	
	/// IStageOutputSerializer override
	void OpenDocToSave(const std::wstring& path) override;
	void Finalize() override;
	///

private:

	WValue SerializeNode(std::shared_ptr<AbstractTreeNode> node) const;

	WDocument m_docToSave;
	WDocument::AllocatorType* m_allocator;

	std::wofstream m_resFile;
};