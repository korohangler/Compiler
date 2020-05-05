#pragma once
#include "CompilerParts/Observers/IObserver.h"
#include "IStageOutputSerializer.h"
#include "CompilerParts/StageOutputStructs.h"

class __declspec(dllexport) ParserStageOutputSerializer : public IStageOutputSerializer, public IObserver<std::shared_ptr<AbstractTreeNode>>
{
public:
	ParserStageOutputSerializer();
	
	/// IObserver override
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
