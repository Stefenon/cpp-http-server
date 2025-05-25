#include "router/HttpRouter.h"

HttpRouter::HttpRouter()
{
	get_routes.reserve(100);
	post_routes.reserve(100);
}

void HttpRouter::get(const std::string_view& uri, const EndpointFunction& endpoint_function)
{
	Route route(uri, endpoint_function);
	get_routes.emplace_back(route);
}

void HttpRouter::post(const std::string_view& uri, const EndpointFunction& endpoint_function)
{
	Route route(uri, endpoint_function);
	post_routes.emplace_back(route);
}

void HttpRouter::add_route(const Http::Method& method, const std::string_view& uri, const EndpointFunction& endpoint_function)
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

std::pair<EndpointFunction, std::unordered_map<std::string, std::string>> HttpRouter::get_endpoint_function(const Http::Method& method, const std::string_view& uri) const
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

	std::unordered_map<std::string, std::string> path_params;
	
	for (const Route& route : routes_to_search) {
		int sub_path_idx = 0;
		size_t route_subpath_start = 1;
		size_t route_subpath_end = route.uri.substr(1).find("/");

		size_t request_subpath_start = 1;
		size_t request_subpath_end = uri.substr(1).find("/");

		while (true)
		{
			std::string_view route_subpath = route.uri.substr(route_subpath_start, route_subpath_end - route_subpath_start);
			std::string_view request_subpath = uri.substr(request_subpath_start, request_subpath_end - request_subpath_start);

			if ((route_subpath_end == std::string_view::npos && request_subpath_end != std::string_view::npos) || 
				(route_subpath_end != std::string_view::npos && request_subpath_end == std::string_view::npos)
			) {
				break;
			}

			if (route_subpath.at(0) == '{') {
				std::string param_key = StringFormatting::find_between_characters(std::string(route_subpath), '{', '}');
				path_params[param_key] = request_subpath;
			}
			else if (route_subpath != request_subpath) {
				path_params.clear();
				break;
			}

			if (route_subpath_end == std::string_view::npos && request_subpath_end == std::string_view::npos) {
				return { route.endpoint_function, path_params };
			}

			route_subpath_start = route_subpath_end + 1;
			route_subpath_end = route.uri.find("/", route_subpath_start);
			request_subpath_start = request_subpath_end + 1;
			request_subpath_end = uri.find("/", request_subpath_start);
		}
	}

	throw EndpointNotFoundException("No endpoint found for URI " + std::string(uri) + "and method " + Http::get_string_from_method(method));
}
