#pragma once

#include <unordered_map>

#include "json/json.hpp"
#include "Response.h"

using json = nlohmann::json;

class JsonResponse : public Response {
	public:
		std::unordered_map<std::string, std::string> headers;

		JsonResponse(json& json_content, HttpStatusCode new_status_code);
		std::string content_to_string() const override;
		void set_content_from_string(std::string& new_content) override;
		json json_content;
	protected:
};
