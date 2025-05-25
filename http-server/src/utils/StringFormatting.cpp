#include "utils/StringFormatting.h"

namespace StringFormatting {
	std::string to_lower(const std::string& string_to_convert)
	{
		std::string converted_string = string_to_convert;
	
		std::transform(converted_string.begin(), converted_string.end(), converted_string.begin(),
			[](unsigned char c) { return std::tolower(c); });

		return converted_string;
	}

	std::string trim(const std::string& str, const std::string& whitespace)
	{
		const auto str_begin = str.find_first_not_of(whitespace);
		if (str_begin == std::string::npos)
			return "";

		const auto str_end = str.find_last_not_of(whitespace);
		const auto str_range = str_end - str_begin + 1;

		return str.substr(str_begin, str_range);
	}

	std::string find_between_characters(const std::string& str, const char start_char, const char end_char)
	{
		size_t start_pos = str.find(start_char);
		if (start_pos == std::string::npos) {
			return "";
		}
		start_pos += 1;

		size_t end_pos = str.find(end_char, start_pos);
		if (end_pos == std::string::npos) {
			return "";
		}

		return str.substr(start_pos, end_pos - start_pos);
	}

}
