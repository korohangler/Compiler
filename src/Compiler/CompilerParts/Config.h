#pragma once

struct Config
{
	std::wstring InputFileName;
	std::wstring OutputFileName = L"a.json";

	std::wstring FirstStage = L"Lexer";
	std::wstring LastStage  = L"Lexer";

	std::wstring ExecutionFolder;
};