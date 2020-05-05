#include "stdafx.h"
#include "Compiler.h"
#include "Stages/LexerStage.h"
#include "Stages/ParserStage.h"
#include "Stages/CodeGeneratorStage.h"
#include "Stages/SemanticAnalyzer.h"
#include "Stages/StageOutputSerializers/LexerStageOutputSerializer.h"
#include "Stages/StageOutputSerializers/ParserStageOutputSerializer.h"
#include "Stages/StageOutputSerializers/IdentificatorTableSerializer.h"

Compiler::Compiler(Config config) : m_config(std::move(config))
{
	ASSERT2(std::filesystem::exists(m_config.InputFileName), std::wstring(L"Specified file doesn't exist: ") + m_config.InputFileName)

	// Stages
	auto lexer            = std::make_shared<LexerStage>(m_config.ExecutionFolder, m_config.InputFileName, m_config.NeedLog);
	auto parser           = std::make_shared<ParserStage>();
	auto semanticAnalyzer = std::make_shared<SemanticAnalyzer>();
	auto codeGenerator    = std::make_shared<CodeGeneratorStage>();

	// Serializers
	auto lexerSerializer   = std::make_shared<LexerStageOutputSerializer>();
	auto parserSerializer  = std::make_shared<ParserStageOutputSerializer>();
	auto IDTableSerializer = std::make_shared<IdentificatorTableSerializer>();

	// Bind to lexer
	lexer->RegisterListener(lexerSerializer.get());
	lexer->RegisterListener(parser.get());

	// Bind to parser
	parser->RegisterListener(parserSerializer.get());
	parser->RegisterListener(semanticAnalyzer.get());

	// Bind to semanticAnalyzer
	semanticAnalyzer->RegisterListener(codeGenerator.get());
	semanticAnalyzer->RegisterListener(IDTableSerializer.get());
	
	m_stages.push_back(lexer);
	m_stages.push_back(parser);
	m_stages.push_back(semanticAnalyzer);
	m_stages.push_back(codeGenerator);

	m_stageOutputSerializers.push_back(lexerSerializer);
	m_stageOutputSerializers.push_back(parserSerializer);
	m_stageOutputSerializers.push_back(IDTableSerializer);
}

void Compiler::PerformCompilation()
{
	PerformStages();
}

void Compiler::PerformStages()
{
	for (auto& serializer : m_stageOutputSerializers) serializer->OpenDocToSave(m_config.OutputFileName);

	for (auto& stage : m_stages) stage->DoStage();

	for (auto& serializer : m_stageOutputSerializers) serializer->Finalize();
}
