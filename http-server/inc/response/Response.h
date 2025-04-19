#pragma once

#include <string>

#include "status/StatusCodes.h"

class Response {
	public:
		std::string content;
		HttpStatusCode status_code;
		std::string content_type;

		Response(std::string new_content, HttpStatusCode new_status_code, std::string new_content_type) : content(new_content), status_code(new_status_code), content_type(new_content_type) {}

	protected:
		virtual std::string content_to_string() const;
		Response(std::string new_content, HttpStatusCode new_status_code) : content(new_content), status_code(new_status_code) {}
};