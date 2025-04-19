#pragma once

#include <unordered_map>

#include "json/json.hpp"
#include "Response.h"

using json = nlohmann::json;

class JsonResponse : Response {
	public:
		const std::string content_type = "application/json";
		json content;
		std::unordered_map<std::string, std::string> headers;

		JsonResponse(json new_content, HttpStatusCode new_status_code) : Response(new_content, new_status_code) {}
	protected:
		std::string content_to_string() const override;
};
