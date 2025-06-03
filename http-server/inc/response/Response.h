#pragma once

#include "status/StatusCodes.h"

#include <string>
#include <unordered_map>

class Response
{
public:
	HttpStatusCode status_code = HttpStatusCode::HTTP_200_OK;
	std::unordered_multimap<std::string, std::string> headers;
	std::string content_type = "";
	std::string content = "";

	Response() = default;
	Response(HttpStatusCode new_status_code) : status_code(new_status_code) {}
	Response(std::string new_content, HttpStatusCode new_status_code) : content(new_content), status_code(new_status_code) {}
	Response(std::string new_content, HttpStatusCode new_status_code, std::string new_content_type) : content(new_content), status_code(new_status_code), content_type(new_content_type) {}
	void set_header(std::string key, std::string value);
	virtual void set_content_from_string(std::string &new_content);
	virtual std::string content_to_string() const;
};