#pragma once

#include <vector>
#include <string>

typedef std::string (*EndpointFunctionPtr)(std::string body);

struct Route {
	std::string uri;
	EndpointFunctionPtr fnc_ptr;

	Route(std::string uri, EndpointFunctionPtr fnc_ptr) : uri(uri), fnc_ptr(fnc_ptr) {}
};

class HttpRouter {
	private:
		std::vector<Route> get_routes;
		std::vector<Route> post_routes;

	public:
		HttpRouter();

		void get(std::string uri, EndpointFunctionPtr fnc_ptr);
		void post(std::string uri, EndpointFunctionPtr fnc_ptr);
};