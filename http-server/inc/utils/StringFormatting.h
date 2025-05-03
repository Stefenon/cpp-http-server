#pragma once

#include <algorithm>
#include <cctype>
#include <string>

constexpr std::string CRLF = "\r\n";

namespace StringFormatting {
	std::string to_lower(std::string string_to_convert);
	std::string trim(const std::string& str, const std::string& whitespace = " \t");
}