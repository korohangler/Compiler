#pragma once
#include "CompilerParts/Config.h"
#include "Stages/IStage.h"
#include "Stages/StageOutputSerializers/IStageOutputSerializer.h"

class Compiler
{
public:
	Compiler();
	Compiler(Config config);
	~Compiler();

	void PerformCompilation();

private:

	void SetFirstStageIdx();

	void CheckConfig();

	void PerformStages();

	Config			  m_config;
	size_t			  m_currentStageIdx;
	size_t			  m_firstStageIdx;

	std::vector<IStage*> m_stages;
	std::vector<IStageOutputSerializer*> m_stageOuputSerializers;
};