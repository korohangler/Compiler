#include "stdafx.h"
#include "Compiler.h"
#include "Stages/LexerStage.h"
#include "Stages/ParserStage.h"
#include "Stages/StageOutputSerializers/LexerStageOutputSerializer.h"
#include "Stages/StageOutputSerializers/ParserStageOutputSerializer.h"

Compiler::Compiler() : m_currentStageIdx(0), m_firstStageIdx(0)
{
}

Compiler::Compiler(Config config) : Compiler()
{
	m_config = config;
	CheckConfig();

	LexerStage* lexer = new LexerStage(m_config.ExecutionFolder, m_config.InputFileName, m_config.NeedLog);
	LexerStageOutputSerializer* lexerSerializer = new LexerStageOutputSerializer();
	lexer->RegisterListener(lexerSerializer);
	m_stages.push_back(lexer);
	m_stageOuputSerializers.push_back(lexerSerializer);

	ParserStage* parser = new ParserStage(config.ExecutionFolder + L"/../../../data/Parser");
	lexer->RegisterListener(parser);
	ParserStageOutputSerializer* parserSerializer = new ParserStageOutputSerializer();
	parser->RegisterListener(parserSerializer);
	m_stages.push_back(parser);
	m_stageOuputSerializers.push_back(parserSerializer);
}

Compiler::~Compiler()
{
	for (auto& a : m_stages)
		delete a;

	for (auto& a : m_stageOuputSerializers)
		delete a;
}

void Compiler::PerformCompilation()
{
	SetFirstStageIdx();
	PerformStages();
}

void Compiler::SetFirstStageIdx()
{
	if (!m_config.FirstStage.empty())
	{
		m_firstStageIdx =  std::find_if(m_stages.begin(), m_stages.end(), [&](IStage* a) { return a->GetStageName() == m_config.FirstStage; }) - m_stages.begin();
		m_currentStageIdx = m_firstStageIdx;
	}
}

void Compiler::CheckConfig()
{
	ASSERT2(std::filesystem::exists(m_config.InputFileName), std::wstring(L"Specified file doesn't exist: ") + m_config.InputFileName)
}

void Compiler::PerformStages()
{
	for (auto& serializer : m_stageOuputSerializers)
		serializer->OpenDocToSave(m_config.OutputFileName);

	for (size_t i = m_firstStageIdx; i < m_stages.size(); i++)
	{
		m_stages[i]->DoStage();

		if (m_stages[i]->GetStageName() == m_config.LastStage)
			break;
	}

	for (auto& serializer : m_stageOuputSerializers)
		serializer->Finalize();
}
