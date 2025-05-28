#pragma once

#include <vector>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "HtmlElement.h"

class HtmlDocument {
	public:
		HtmlElement head = HtmlElement("head");
		HtmlElement body = HtmlElement("body");

		HtmlDocument() = default;

		std::string to_string() const;
};
