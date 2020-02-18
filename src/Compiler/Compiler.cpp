#include "stdafx.h"
#include "Compiler.h"
#include "Stages/LexerStage.h"

Compiler::Compiler() : m_currentStageIdx(0), m_firstStageIdx(0)
{
}

Compiler::Compiler(Config config) : Compiler()
{
	m_config = config;
	m_stages.push_back(std::make_shared<LexerStage>(m_config.ExecutionFolder));
	m_stageOutputs.emplace_back();
}

void Compiler::PerformCompilation()
{
	CheckConfig();
	OpenFileForStage();
	SetFirstStageIdx();
	PerformStages();
}

void Compiler::SetFirstStageIdx()
{
	if (m_config.FirstStage != L"")
	{
		m_firstStageIdx =  std::find_if(m_stages.begin(), m_stages.end(), [&](std::shared_ptr<IStage> a) { return a->GetStageName() == m_config.FirstStage; }) - m_stages.begin();
		m_currentStageIdx = m_firstStageIdx;
	}
}

void Compiler::OpenFileForStage()
{
	m_inputFile.open(m_config.InputFileName);
	ASSERT2(m_inputFile.is_open(), std::wstring(L"Unable to open specified file: ") + m_config.InputFileName);

	m_outputFile.open(m_config.OutputFileName);
	ASSERT2(m_outputFile.is_open(), std::wstring(L"Unable to open specified file: ") + m_config.OutputFileName);
}

void Compiler::CheckConfig()
{
	ASSERT2(std::filesystem::exists(m_config.InputFileName), std::wstring(L"Specified file doesn't exist: ") + m_config.InputFileName)

	if (m_config.FirstStage != L"")
	{
		auto foundStage = std::find_if(m_stages.begin(), m_stages.end(), [&](std::shared_ptr<IStage> a) { return a->GetStageName() == m_config.FirstStage; });

		ASSERT2(foundStage != m_stages.end(), std::wstring(L"Can't find first stage: ") + m_config.FirstStage);
	}

	if (m_config.LastStage != L"")
	{
		auto foundStage = std::find_if(m_stages.begin(), m_stages.end(), [&](std::shared_ptr<IStage> a) { return a->GetStageName() == m_config.LastStage; });

		ASSERT2(foundStage != m_stages.end(), std::wstring(L"Can't find first stage: ") + m_config.LastStage);
	}
}

void Compiler::PerformStages()
{
	bool doStages = false;

	for (size_t i = m_firstStageIdx; i < m_stages.size(); i++)
	{
		std::wistream* currStageInput = &m_inputFile;
		if (i == m_firstStageIdx)
			doStages = true;
		else
			currStageInput = &m_stageOutputs[i - 1];

		if (m_stages[i]->GetStageName() == m_config.LastStage)
		{
			m_stages[i]->DoStage(*currStageInput, m_outputFile);
			break;
		}

		if (doStages)
			m_stages[i]->DoStage(*currStageInput, m_stageOutputs[i]);
	}
}
