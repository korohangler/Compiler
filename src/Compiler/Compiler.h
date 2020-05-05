#pragma once
#include <vector>

#include "CompilerParts/Config.h"
#include "Stages/IStage.h"
#include "Stages/StageOutputSerializers/IStageOutputSerializer.h"

class __declspec(dllexport) Compiler
{
public:
	Compiler() = delete;
	Compiler(Config config);
	~Compiler() = default;

	void PerformCompilation();

private:
	Config			  m_config;

	std::vector<std::shared_ptr<IStage>> m_stages;
	std::vector<std::shared_ptr<IStageOutputSerializer>> m_stageOutputSerializers;
};
