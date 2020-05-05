#include "stdafx.h"
#include "Compiler.h"
#include "Stages/LexerStage.h"
#include "Stages/ParserStage.h"
#include "Stages/StageOutputSerializers/LexerStageOutputSerializer.h"
#include "Stages/StageOutputSerializers/ParserStageOutputSerializer.h"
#include "Stages/CodeGeneratorStage.h"
#include "Stages/SemanticAnalyzer.h"

Compiler::Compiler(Config config) : m_config(std::move(config))
{
	ASSERT2(std::filesystem::exists(m_config.InputFileName), std::wstring(L"Specified file doesn't exist: ") + m_config.InputFileName)

	// Stages
	std::shared_ptr<LexerStage>		    lexer = std::make_shared<LexerStage>(m_config.ExecutionFolder, m_config.InputFileName, m_config.NeedLog);
	std::shared_ptr<ParserStage>	    parser = std::make_shared<ParserStage>();
	std::shared_ptr<SemanticAnalyzer>   semanticAnalyzer = std::make_shared<SemanticAnalyzer>();
	std::shared_ptr<CodeGeneratorStage> codeGenerator = std::make_shared<CodeGeneratorStage>();

	// Serializers
	std::shared_ptr<LexerStageOutputSerializer>  lexerSerializer = std::make_shared<LexerStageOutputSerializer>();
	std::shared_ptr<ParserStageOutputSerializer> parserSerializer = std::make_shared<ParserStageOutputSerializer>();

	// Bind to lexer
	lexer->RegisterListener(lexerSerializer.get());
	lexer->RegisterListener(parser.get());

	// Bind to parser
	parser->RegisterListener(parserSerializer.get());
	parser->RegisterListener(semanticAnalyzer.get());

	// Bind to semanticAnalyzer
	semanticAnalyzer->RegisterListener(codeGenerator.get());
	
	m_stages.push_back(lexer);
	m_stages.push_back(parser);
	m_stages.push_back(semanticAnalyzer);
	m_stages.push_back(codeGenerator);

	m_stageOuputSerializers.push_back(lexerSerializer);
	m_stageOuputSerializers.push_back(parserSerializer);
}

void Compiler::PerformCompilation()
{
	PerformStages();
}

void Compiler::PerformStages()
{
	for (auto& serializer : m_stageOuputSerializers) serializer->OpenDocToSave(m_config.OutputFileName);

	for (auto& stage : m_stages) stage->DoStage();

	for (auto& serializer : m_stageOuputSerializers) serializer->Finalize();
}
