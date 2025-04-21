#include "response/Response.h"

std::string Response::content_to_string() const
{
	return content;
}

void Response::set_content_from_string(std::string& new_content)
{
	content = new_content;
}
