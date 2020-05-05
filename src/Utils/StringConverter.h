#pragma once
#include <string>

class StringConverter
{
public:
	[[nodiscard]] static std::wstring StrToWstr(std::wstring str) { return std::wstring(str); }

	static std::wstring StrToWstr(std::string str) 
	{ 
		std::wostringstream stream;
		stream << str.c_str();
		return std::wstring(stream.str());
	}

	[[nodiscard]] static std::string WstrToStr(std::string str) { return str; }
	[[nodiscard]] static std::string WstrToStr(std::wstring str) { return std::string(str.begin(), str.end()); }

};
