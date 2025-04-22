#pragma once

#include <string>
#include <unordered_map>
#include <vector>

enum HttpMethod {
	GET,
	POST,
	PUT,
	PATCH,
	DELETE,
	HEAD,
	OPTIONS,
	CONNECT,
	TRACE,
	INVALID_METHOD
};

class Request {
	protected:
		std::string uri;
		HttpMethod method;
		std::unordered_map<std::string, std::string> single_value_query_params;
		std::unordered_map<std::string, std::vector<std::string>> multi_value_query_params;
		std::unordered_map<std::string, std::string> headers;
	public:
		Request(std::string request_str);
		std::string get_uri();
		HttpMethod get_method() const;
		static std::string get_string_from_method(HttpMethod method);
		static HttpMethod get_method_from_string(std::string method_string);
		std::unordered_map<std::string, std::string> get_single_value_query_params();
		std::unordered_map<std::string, std::vector<std::string>> get_multi_value_query_params();
		std::unordered_map<std::string, std::string> get_headers();
};
