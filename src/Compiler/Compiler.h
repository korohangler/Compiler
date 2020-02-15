#pragma once
#include "CompilerParts/Config.h"
#include "Stages/IStage.h"

class Compiler
{
public:
	Compiler();
	Compiler(Config config);

	void PerformCompilation();

private:

	void SetFirstStageIdx();

	void OpenFileForStage();

	void CheckConfig();

	void PerformStages();

	Config			  m_config;
	std::wifstream	  m_inputFile;
	std::wofstream	  m_outputFile;
	size_t			  m_currentStageIdx;
	size_t			  m_firstStageIdx;

	std::vector<std::wstringstream>		 m_stageOutputs;
	std::vector<std::shared_ptr<IStage>> m_stages;
};