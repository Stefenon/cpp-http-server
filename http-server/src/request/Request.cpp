#include "request/Request.h"

Request::Request(std::string request_str)
{
}

std::string Request::get_uri() {
	return uri;
}

HttpMethod Request::get_method() const {
	return method;
}

std::string Request::get_string_from_method(HttpMethod method) {
	switch (method) {
		case HttpMethod::GET:
			return "GET";
		case HttpMethod::POST:
			return "POST";
		case HttpMethod::PUT:
			return "PUT";
		case HttpMethod::PATCH:
			return "PATCH";
		case HttpMethod::DELETE:
			return "DELETE";
		case HttpMethod::HEAD:
			return "HEAD";
		case HttpMethod::OPTIONS:
			return "OPTIONS";
		case HttpMethod::CONNECT:
			return "CONNECT";
		case HttpMethod::TRACE:
			return "TRACE";
		case HttpMethod::INVALID_METHOD:
			return "INVALID_METHOD";
	}
}

HttpMethod Request::get_method_from_string(std::string method_string) {
	if (method_string == "GET")
		return HttpMethod::GET;
	else if (method_string == "POST")
		return HttpMethod::POST;
	else if (method_string == "PUT")
		return HttpMethod::PUT;
	else if (method_string == "PATCH")
		return HttpMethod::PATCH;
	else if (method_string == "DELETE")
		return HttpMethod::DELETE;
	else if (method_string == "HEAD")
		return HttpMethod::HEAD;
	else if (method_string == "OPTIONS")
		return HttpMethod::OPTIONS;
	else if (method_string == "CONNECT")
		return HttpMethod::CONNECT;
	else if (method_string == "TRACE")
		return HttpMethod::TRACE;
	else
		return HttpMethod::INVALID_METHOD;
}

std::unordered_map<std::string, std::string> Request::get_single_value_query_params() {
	return single_value_query_params;
}

std::unordered_map<std::string, std::vector<std::string>> Request::get_multi_value_query_params() {
	return multi_value_query_params;
}

std::unordered_map<std::string, std::string> Request::get_headers() {
	return headers;
}
