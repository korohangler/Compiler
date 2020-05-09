#include "stdafx.h"
#include "ASMCompilerStage.h"

void ASMCompilerStage::Notify(std::shared_ptr<ASMConstructor> constructor)
{
	const std::wstring asmFileName(m_outFileName + L".asm");
	std::wofstream asmFile(asmFileName);

	asmFile << constructor->GenerateASM();
	asmFile.close();

	std::wstring command = L"\"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Auxiliary/Build/vcvarsamd64_x86.bat\"";

	// Create obj file
	command += L" && ml ";
	command += asmFileName;

	// Link obj to .exe
	command += L" && cl ";
	command += m_outFileName + L".obj";
	command += std::wstring(L" /Fe:") + m_outFileName + L".exe";
	
	_wsystem(command.c_str());
}
