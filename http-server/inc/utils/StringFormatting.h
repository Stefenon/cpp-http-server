#pragma once

#include <algorithm>
#include <cctype>
#include <string>

namespace StringFormatting {
	std::string to_lower(std::string string_to_convert);
	std::string trim(const std::string& str, const std::string& whitespace = " \t");
}