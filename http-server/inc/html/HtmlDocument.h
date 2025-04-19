#pragma once
#include <vector>
#include <filesystem>
#include <sstream>
#include <fstream>

#include "HtmlElement.h"

class HtmlDocument {
	public:
		HtmlElement head = HtmlElement("head");
		HtmlElement body = HtmlElement("body");

		HtmlDocument() = default;
		HtmlDocument(const std::filesystem::path& html_file_path);

		std::string to_string();
};
