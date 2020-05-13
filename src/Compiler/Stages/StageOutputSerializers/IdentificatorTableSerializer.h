#pragma once
#include "CompilerParts/IdentificatorTable.h"
#include "CompilerParts/Observers/IObserver.h"
#include "IStageOutputSerializer.h"
#include "StageOutputStructs/AbstractTreeNode.h"

class IdentificatorTableSerializer : public IStageOutputSerializer, public IObserver<std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>>>
{
public:
	IdentificatorTableSerializer();

	/// IObserver override
	void Notify(std::pair<std::shared_ptr<AbstractTreeNode>, std::shared_ptr<IdentificatorTable>> data) override;
	///

	/// IStageOutputSerializer override
	void OpenDocToSave(const std::wstring_view path) override;

	void Finalize() override {};
	///

private:

	WDocument m_docToSave;

	WDocument::AllocatorType* m_allocator;

	std::wofstream m_resFile;
};
