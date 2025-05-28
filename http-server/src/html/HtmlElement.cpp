#include "html/HtmlElement.h"

void HtmlElement::check_if_tag_is_valid(std::string tag_name)
{
	if (tag_name == "body")
	{
		throw std::runtime_error("Element with tag 'body' not allowed as child element.");
	}
	else if (tag_name == "head")
	{
		throw std::runtime_error("Element with tag 'head' not allowed as child element.");
	}
}

void HtmlElement::set_text(std::string new_text)
{
	auto it = std::find(VOID_ELEMENTS.begin(), VOID_ELEMENTS.end(), tag);
	if (it != VOID_ELEMENTS.end())
	{
		throw std::runtime_error("Void element " + tag + " does not allow text content");
	}

	text = new_text;
}

std::string HtmlElement::get_text()
{
	return text;
}

void HtmlElement::set_tail(std::string new_tail)
{
	tail = new_tail;
}

std::string HtmlElement::get_tail()
{
	return tail;
}

void HtmlElement::add_child_element(HtmlElement new_element)
{
	auto it = std::find(VOID_ELEMENTS.begin(), VOID_ELEMENTS.end(), tag);
	if (it != VOID_ELEMENTS.end())
	{
		throw std::runtime_error("Void element " + tag + " does not allow child elements");
	}

	check_if_tag_is_valid(new_element.tag);
	children.emplace_back(new_element);
}

void HtmlElement::remove_child_element(int index)
{
	children.erase(children.begin() + index);
}

std::vector<HtmlElement> HtmlElement::get_child_elements()
{
	return children;
}

void HtmlElement::set_attribute(std::string name, std::string value)
{
	attributes[name] = value;
}

std::string HtmlElement::to_string() const
{
	// Initialize HTML component
	std::string element_str = "<" + tag;
	// Add attributes
	for (auto &it : attributes)
	{
		element_str += " " + it.first + "=" + it.second;
	}

	if (!text.empty() || children.size() > 0)
	{
		// Add text
		element_str += ">" + text;

		// Add children elements
		for (const HtmlElement &child : children)
		{
			element_str += child.to_string();
		}

		// Close element tag
		element_str += "</" + tag + ">";
	}
	else
	{
		element_str += "/>";
	}

	// Add tail to element
	if (!tail.empty())
	{
		element_str += tail;
	}

	return element_str;
}
