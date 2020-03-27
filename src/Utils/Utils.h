#pragma once

namespace Utils
{
#include "StringConverter.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define ASSERT(msg) throw std::runtime_error(Utils::StringConverter::WstrToStr(msg));

#define ASSERT2(expression, msg) if(!(expression)) throw std::runtime_error(Utils::StringConverter::WstrToStr(msg));

	std::vector<wchar_t> ReadFile(std::wstring_view pathToFile)
	{
		std::wifstream file(pathToFile.data());

		ASSERT2(file.is_open(), std::wstring(L"Can't open file: ") + pathToFile.data());

		return std::vector<wchar_t>(std::istreambuf_iterator<wchar_t>(file), std::istreambuf_iterator<wchar_t>());
	}

	class Logger
	{
	public:
		static void LogProcess(const std::string& msg, size_t currItem, size_t size, bool returnCarret = false) { std::cout << msg << ' ' << (currItem + 1) * 100 / size << (returnCarret ? "%\r" : "%\n") << std::flush; }
		static void LogProcess(const std::wstring& msg, size_t currItem, size_t size, bool returnCarret = false) { std::wcout << msg << L' ' << (currItem + 1) / size << '%' << (returnCarret ? L'\r' : L'\n') << std::flush; }

		static void Log(const std::string& msg, bool returnCaret = false) { std::cout << msg << (returnCaret ? '\r' : '\n') << std::flush; }
	};
}