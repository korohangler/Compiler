#pragma once
#include "Config.h"

class ConfigReader
{
public:

	Config ReadConfigFromArgv(int argc, char* argv[]);

private:

	void ProcessOption(int argc, char* argv[]);

	Config m_config;

	int m_currentOptionIdx;
};
