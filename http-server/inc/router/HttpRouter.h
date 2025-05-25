#pragma once

#include <vector>
#include <string>
#include <functional>

#include "request/Request.h"
#include "response/Response.h"
#include "utils/HttpMethods.h"
#include "exceptions/CustomExceptions.h"

using EndpointFunction = std::function<Response(Request)>;

class Route {
	public:
		std::string_view uri;
		EndpointFunction endpoint_function;

		Route(std::string_view new_uri, EndpointFunction new_endpoint_function) : uri(new_uri), endpoint_function(new_endpoint_function) {}
};

class HttpRouter {
	private:
		std::vector<Route> get_routes;
		std::vector<Route> post_routes;

	public:
		HttpRouter();

		void get(const std::string_view& uri, const EndpointFunction& endpoint_function);
		void post(const std::string_view& uri, const EndpointFunction& endpoint_function);
		void add_route(const Http::Method& method, const std::string_view& uri, const EndpointFunction& endpoint_function);
		std::pair<EndpointFunction, std::unordered_map<std::string, std::string>> get_endpoint_function(const Http::Method& method, const std::string_view& uri) const;
};