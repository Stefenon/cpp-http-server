#include "html/HtmlDocument.h"

HtmlDocument::HtmlDocument(const std::filesystem::path& html_file_path)
{
	std::ifstream f(html_file_path);
	std::stringstream buffer;
	buffer << f.rdbuf();

	std::string doc_str = buffer.str();
}

std::string HtmlDocument::to_string()
{
	std::string doc_str = "<!DOCTYPE html> <html>";
	doc_str += head.to_string();
	doc_str += body.to_string();
	doc_str += "</html>";

	return doc_str;
}
