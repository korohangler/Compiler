#include "stdafx.h"

#define BOOST_TEST_MODULE mytests

#include <boost/test/included/unit_test.hpp>
#include "Stages/LexerStage.h"
#include "CompilerParts/ConfigReader.h"

Config TestConfig;

using namespace boost::unit_test;


BOOST_AUTO_TEST_CASE(myTestCase)
{
	ConfigReader reader;

	TestConfig = reader.ReadConfigFromArgv(framework::master_test_suite().argc, framework::master_test_suite().argv);

}
