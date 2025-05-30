#pragma once

#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

#include "request/Request.h"
#include "response/Response.h"
#include "utils/HttpMethods.h"
#include "exceptions/CustomExceptions.h"

using EndpointFunction = std::function<Response(Request)>;

class Route
{
public:
	std::string_view uri;
	std::unordered_map<Http::Method, EndpointFunction> endpoint_functions;

	Route(const std::string_view &new_uri, const Http::Method &new_method, const EndpointFunction &new_endpoint_function);
};

class HttpRouter
{
private:
	std::vector<Route> routes;

public:
	HttpRouter();

	void get(const std::string_view &uri, const EndpointFunction &endpoint_function);
	void post(const std::string_view &uri, const EndpointFunction &endpoint_function);
	void put(const std::string_view &uri, const EndpointFunction &endpoint_function);
	void patch(const std::string_view &uri, const EndpointFunction &endpoint_function);
	void del(const std::string_view &uri, const EndpointFunction &endpoint_function);
	void head(const std::string_view &uri, const EndpointFunction &endpoint_function);
	void options(const std::string_view &uri, const EndpointFunction &endpoint_function);
	void connect(const std::string_view &uri, const EndpointFunction &endpoint_function);
	void trace(const std::string_view &uri, const EndpointFunction &endpoint_function);
	void add_route(const Http::Method &method, const std::string_view &uri, const EndpointFunction &endpoint_function);
	std::pair<EndpointFunction, std::unordered_map<std::string, std::string>> get_endpoint_function(const Http::Method &method, const std::string_view &uri) const;
};