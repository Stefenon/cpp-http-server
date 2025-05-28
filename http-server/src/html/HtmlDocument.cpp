#include "html/HtmlDocument.h"

std::string HtmlDocument::to_string() const
{
	std::string doc_str = "<html>";
	doc_str += head.to_string();
	doc_str += body.to_string();
	doc_str += "</html>";

	return doc_str;
}
