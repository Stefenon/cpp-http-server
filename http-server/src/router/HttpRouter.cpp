#include "router/HttpRouter.h"

HttpRouter::HttpRouter()
{
	get_routes.reserve(100);
	post_routes.reserve(100);
}

void HttpRouter::get(std::string uri, EndpointFunctionPtr fnc_ptr)
{
	Route route(uri, fnc_ptr);
	get_routes.emplace_back(route);
}

void HttpRouter::post(std::string uri, EndpointFunctionPtr fnc_ptr)
{
	Route route(uri, fnc_ptr);
	post_routes.emplace_back(route);
}