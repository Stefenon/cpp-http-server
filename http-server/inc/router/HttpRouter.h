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
		std::string uri;
		EndpointFunction endpoint_function;

		Route(std::string new_uri, EndpointFunction new_endpoint_function) : uri(new_uri), endpoint_function(new_endpoint_function) {}
};

class HttpRouter {
	private:
		std::vector<Route> get_routes;
		std::vector<Route> post_routes;

	public:
		HttpRouter();

		void get(std::string uri, EndpointFunction endpoint_function);
		void post(std::string uri, EndpointFunction endpoint_function);
		void add_route(Http::Method method, std::string uri, EndpointFunction endpoint_function);
		EndpointFunction get_endpoint_function(const Http::Method method, const std::string& uri) const;
};