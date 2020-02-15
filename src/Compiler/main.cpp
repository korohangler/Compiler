#include "stdafx.h"
#include "Compiler.h"
#include "CompilerParts/ConfigReader.h"

int main(int argc, char*argv[])
{
	setlocale(LC_ALL, "en_US.UTF-8");

	try
	{
		ConfigReader confReader;

		Compiler compiler(confReader.ReadConfigFromArgv(argc, argv));

		compiler.PerformCompilation();
	}
	catch (std::runtime_error err)
	{
		std::cout << err.what() << std::endl;
		std::cout << "Unable to compile file!\n";
		return -1;
	}

	return 0;
}
