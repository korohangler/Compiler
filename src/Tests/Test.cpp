#include "stdafx.h"

#include "Stages/LexerStage.h"
#include "CompilerParts/ConfigReader.h"
#include "LexerTester.h"

Config TestConfig;

using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(myTestCase)
{
	ConfigReader reader;

	std::string executionPath = framework::master_test_suite().argv[0];

	const char* argv[2] = {
		framework::master_test_suite().argv[0],
		"../data/TestData/autotests/Lexer/leta.js"
	};
	
	TestConfig = reader.ReadConfigFromArgv(2, argv);

	LexerStage lexer(TestConfig.ExecutionFolder, TestConfig.InputFileName, false);

	std::vector<Token> expectedTokens = {
		Token(L"let", L"Keyword", 0)
		, Token(L" ", L"CommonSeparator", 0)
		, Token(L"a", L"Identificator", 0)
		, LexerStage::FinalToken
	};
	
	std::shared_ptr<LexerTester> tester = std::make_shared<LexerTester>(expectedTokens);
	lexer.RegisterListener(tester.get());
	lexer.DoStage();
}
