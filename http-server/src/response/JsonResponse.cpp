#include "response/JsonResponse.h"

JsonResponse::JsonResponse(json &new_content, HttpStatusCode new_status_code) : Response(new_content.dump(), new_status_code, "application/json")
{
	json_content = new_content;
}

void JsonResponse::set_content_from_string(std::string &new_content)
{
	Response::set_content_from_string(new_content);
	json_content = json::parse(new_content);
}

std::string JsonResponse::content_to_string() const
{
	return json_content.dump();
}
