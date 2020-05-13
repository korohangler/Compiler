#pragma once
#include <string>

class StringConverter
{
public:
	[[nodiscard]] static std::wstring_view StrToWstr(const std::wstring_view str) { return str; }

	static std::wstring StrToWstr(const std::string_view str) 
	{ 
		std::wostringstream stream;
		stream << str.data();
		return std::wstring(stream.str());
	}

	[[nodiscard]] static std::string_view WstrToStr(const std::string_view str) { return str; }
	[[nodiscard]] static std::string WstrToStr(const std::wstring_view str) { return std::string(str.begin(), str.end()); }

};
