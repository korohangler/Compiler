#pragma once
#include "IStage.h"
#include "CompilerParts/Observers/Observable.h"
#include "CompilerParts/ASMConstructor.h"

class ASMCompilerStage : public IStage, public IObserver<std::shared_ptr<ASMConstructor>>
{
public:
	ASMCompilerStage(std::wstring directory, std::wstring outFileName) : m_directory(std::move(directory)), m_outFileName(std::move(outFileName)) {}

	void Notify(std::shared_ptr<ASMConstructor> constructor) override;

	void DoStage() override {};

	[[nodiscard]] std::wstring_view GetStageName() override { return L"ASMCompiler"; }

private:

	std::wstring m_directory;
	std::wstring m_outFileName;
};