#include "router/HttpRouter.h"

Route::Route(const std::string_view &new_uri, const Http::Method &new_method, const EndpointFunction &new_endpoint_function)
{
	uri = new_uri;
	endpoint_functions[new_method] = new_endpoint_function;
}

HttpRouter::HttpRouter()
{
	routes.reserve(100);
}

void HttpRouter::add_route(const Http::Method &method, const std::string_view &uri, const EndpointFunction &endpoint_function)
{
	for (Route &route : routes)
	{
		if (route.uri == uri)
		{
			route.endpoint_functions[method] = endpoint_function;
			return;
		}
	}

	routes.emplace_back(Route(uri, method, endpoint_function));
}

void HttpRouter::get(const std::string_view &uri, const EndpointFunction &endpoint_function)
{
	add_route(Http::Method::GET, uri, endpoint_function);
}

void HttpRouter::post(const std::string_view &uri, const EndpointFunction &endpoint_function)
{
	add_route(Http::Method::POST, uri, endpoint_function);
}
void HttpRouter::put(const std::string_view &uri, const EndpointFunction &endpoint_function)
{
	add_route(Http::Method::PUT, uri, endpoint_function);
}
void HttpRouter::patch(const std::string_view &uri, const EndpointFunction &endpoint_function)
{
	add_route(Http::Method::PATCH, uri, endpoint_function);
}
void HttpRouter::del(const std::string_view &uri, const EndpointFunction &endpoint_function)
{
	add_route(Http::Method::DELETE, uri, endpoint_function);
}
void HttpRouter::head(const std::string_view &uri, const EndpointFunction &endpoint_function)
{
	add_route(Http::Method::HEAD, uri, endpoint_function);
}
void HttpRouter::options(const std::string_view &uri, const EndpointFunction &endpoint_function)
{
	add_route(Http::Method::OPTIONS, uri, endpoint_function);
}
void HttpRouter::connect(const std::string_view &uri, const EndpointFunction &endpoint_function)
{
	add_route(Http::Method::CONNECT, uri, endpoint_function);
}
void HttpRouter::trace(const std::string_view &uri, const EndpointFunction &endpoint_function)
{
	add_route(Http::Method::TRACE, uri, endpoint_function);
}

std::pair<EndpointFunction, std::unordered_map<std::string, std::string>> HttpRouter::get_endpoint_function(const Http::Method &method, const std::string_view &uri) const
{
	std::unordered_map<std::string, std::string> path_params;

	for (const Route &route : routes)
	{
		int sub_path_idx = 0;
		size_t route_subpath_start = 1;
		size_t route_subpath_end = route.uri.find("/", route_subpath_start);

		size_t request_subpath_start = 1;
		size_t request_subpath_end = uri.find("/", request_subpath_start);

		while (true)
		{
			std::string_view route_subpath = route.uri.substr(route_subpath_start, route_subpath_end - route_subpath_start);
			std::string_view request_subpath = uri.substr(request_subpath_start, request_subpath_end - request_subpath_start);

			if (((route_subpath_end == std::string_view::npos || route_subpath_end == route.uri.size() - 1) &&
					 (request_subpath_end != std::string_view::npos && request_subpath_end != uri.size() - 1)) ||
					((route_subpath_end != std::string_view::npos && route_subpath_end != route.uri.size() - 1) &&
					 (request_subpath_end == std::string_view::npos || request_subpath_end == uri.size() - 1)))
			{
				break;
			}

			if (route_subpath.at(0) == '{')
			{
				std::string param_key = StringFormatting::find_between_characters(std::string(route_subpath), '{', '}');
				path_params[param_key] = request_subpath;
			}
			else if (route_subpath != request_subpath)
			{
				path_params.clear();
				break;
			}

			if ((route_subpath_end == std::string_view::npos || route_subpath_end == route.uri.size() - 1) &&
					(request_subpath_end == std::string_view::npos || request_subpath_end == uri.size() - 1))
			{
				auto it = route.endpoint_functions.find(method);
				if (it != route.endpoint_functions.end())
				{
					return {it->second, path_params};
				}

				throw MethodNotAllowedException("Method " + Http::get_string_from_method(method) + " is not allowed for URI " + std::string(uri));
			}

			route_subpath_start = route_subpath_end + 1;
			route_subpath_end = route.uri.find("/", route_subpath_start);
			request_subpath_start = request_subpath_end + 1;
			request_subpath_end = uri.find("/", request_subpath_start);
		}
	}

	throw EndpointNotFoundException("No endpoint found for URI " + std::string(uri));
}
