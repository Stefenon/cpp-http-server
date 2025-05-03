#pragma once

#include <string>

#include "status/StatusCodes.h"

class Response {
	public:
		HttpStatusCode status_code = HttpStatusCode::HTTP_200_OK;
		std::string content_type = "";
		std::string content = "";

		Response() = default;
		Response(HttpStatusCode new_status_code) : status_code(new_status_code) {}
		Response(std::string new_content, HttpStatusCode new_status_code) : content(new_content), status_code(new_status_code) {}
		Response(std::string new_content, HttpStatusCode new_status_code, std::string new_content_type) : content(new_content), status_code(new_status_code), content_type(new_content_type) {}
		virtual void set_content_from_string(std::string& new_content);
		virtual std::string content_to_string() const;
};