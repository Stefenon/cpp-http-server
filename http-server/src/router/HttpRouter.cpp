#include "router/HttpRouter.h"

HttpRouter::HttpRouter()
{
	get_routes.reserve(100);
	post_routes.reserve(100);
}

void HttpRouter::get(std::string uri, EndpointFunction endpoint_function)
{
	Route route(uri, endpoint_function);
	get_routes.emplace_back(route);
}

void HttpRouter::post(std::string uri, EndpointFunction endpoint_function)
{
	Route route(uri, endpoint_function);
	post_routes.emplace_back(route);
}

void HttpRouter::add_route(Http::Method method, std::string uri, EndpointFunction endpoint_function)
{
	switch (method) {
		case Http::Method::GET:
			get(uri, endpoint_function);
			break;
		case Http::Method::POST:
			post(uri, endpoint_function);
			break;
		default:
			throw std::runtime_error("Method " + Http::get_string_from_method(method) + " is not supported yet.");
	}
}

EndpointFunction HttpRouter::get_endpoint_function(const Http::Method method, const std::string& uri) const
{
	std::vector<Route> routes_to_search;

	switch (method) {
	case Http::Method::GET:
		routes_to_search = get_routes;
		break;
	case Http::Method::POST:
		routes_to_search = post_routes;
		break;
	default:
		throw std::runtime_error("Method " + Http::get_string_from_method(method) + " is not supported yet.");
	}

	for (const Route& route : routes_to_search) {
		if (route.uri == uri) {
			return route.endpoint_function;
		}
	}

	throw EndpointNotFoundException("No endpoint found for URI " + uri + "and method " + Http::get_string_from_method(method));
}
