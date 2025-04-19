#include "response/JsonResponse.h"

std::string JsonResponse::content_to_string() const
{
	return content.dump();
}
