#include "stdafx.h"
#include "ConfigReader.h"

Config ConfigReader::ReadConfigFromArgv(int argc, char* argv[])
{
	ASSERT2(argc > 1, "Missing path to source file");

	m_config.ExecutionFolder = std::filesystem::absolute(std::filesystem::path(argv[0])).remove_filename();

	for (m_currentOptionIdx = 1; m_currentOptionIdx < argc; m_currentOptionIdx++)
		ProcessOption(argc, argv);

	return m_config;
}

void ConfigReader::ProcessOption(int argc, char* argv[])
{
	char* opt = argv[m_currentOptionIdx];

	if (strcmp(opt, "-s") == 0)
	{
		ASSERT2(m_currentOptionIdx + 1 < argc, "Unexpected end of command options");

		m_config.LastStage = Utils::StringConverter::StrToWstr(argv[m_currentOptionIdx + 1]);
		m_currentOptionIdx++;
	}
	else if (strcmp(opt, "-o") == 0)
	{
		ASSERT2(m_currentOptionIdx + 1 < argc, "Unexpected end of command options");

		m_config.OutputFileName = Utils::StringConverter::StrToWstr(argv[m_currentOptionIdx + 1]);
		m_currentOptionIdx++;
	}
	else if (strcmp(opt, "-log") == 0)
	{
		m_config.NeedLog = true;
	}
	else
	{
		m_config.InputFileName = Utils::StringConverter::StrToWstr(argv[m_currentOptionIdx]);
	}
}
