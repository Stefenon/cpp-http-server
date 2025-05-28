#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <algorithm>

const std::vector<std::string> VOID_ELEMENTS = {
		"base",
		"br",
		"col",
		"embed",
		"hr",
		"img",
		"input",
		"link",
		"meta",
		"param",
		"source",
		"track",
		"wbr",
};

class HtmlElement
{
private:
	std::string tag;
	std::unordered_map<std::string, std::string> attributes;
	std::vector<HtmlElement> children;
	std::string text;
	std::string tail;

	void check_if_tag_is_valid(std::string tag);

public:
	HtmlElement(std::string new_tag, std::string new_text = "", std::unordered_map<std::string, std::string> new_attributes = {}) : tag(new_tag), text(new_text), attributes(new_attributes) {}

	void set_text(std::string new_text);
	std::string get_text();
	void set_tail(std::string new_tail);
	std::string get_tail();

	void add_child_element(HtmlElement new_element);
	void remove_child_element(int index);
	std::vector<HtmlElement> get_child_elements();
	void set_attribute(std::string name, std::string value);
	std::string to_string() const;
};
