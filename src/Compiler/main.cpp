#include "stdafx.h"

int main()
{
	try
	{
		std::cout << "Hello world!\n";
	}
	catch (std::runtime_error err)
	{
		std::cout << err.what() << std::endl;
		std::cout << "Unable to compile file!\n";
		return -1;
	}

	return 0;
}
