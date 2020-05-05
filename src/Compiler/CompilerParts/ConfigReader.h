#pragma once
#include "Config.h"

class __declspec(dllexport) ConfigReader
{
public:

	Config ReadConfigFromArgv(int argc, const char* argv[]);

private:

	void ProcessOption(int argc, const char* argv[]);

	Config m_config;

	int m_currentOptionIdx;
};
