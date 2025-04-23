#include "request/Request.h"

Request::Request(std::string request_str)
{
	set_method_from_request_str(request_str);
	set_uri_and_query_params_from_request_str(request_str);
	set_headers_from_request_str(request_str);
	set_body_from_request_str(request_str);
}

std::string Request::get_uri() {
	return uri;
}

Http::Method Request::get_method() const {
	return method;
}

void Request::set_method_from_request_str(std::string& request)
{
	size_t split = request.find(" ");
	std::string method_string = request.substr(0, split);
	request = request.substr(split + 1);

	std::cout << "Method: " << method_string << std::endl;

	method = Http::get_method_from_string(method_string);
}

void Request::set_uri_and_query_params_from_request_str(std::string& request)
{
	size_t uri_split = request.find(" ");
	std::string uri_string = request.substr(0, uri_split);
	size_t query_split = uri_string.find("?");
	std::string query_string = uri_string.substr(query_split + 1);
	uri = uri_string.substr(0, query_split);
	std::cout << "URI: " << uri << std::endl;

	query_params = {};

	if (query_split != std::string::npos) {
		size_t param_value_split;
		size_t next_param_split;

		do {
			param_value_split = query_string.find("=");
			std::string param_key = query_string.substr(0, param_value_split);
			next_param_split = query_string.find("&");
			std::string param_value = query_string.substr(param_value_split + 1, next_param_split - (param_value_split + 1));
			query_params.emplace(param_key, param_value);
			query_string = query_string.substr(next_param_split + 1);
		} while (next_param_split != std::string::npos);
	}

	// Remove request line from request string
	size_t end_of_request_line = request.find("\r\n");
	request = request.substr(end_of_request_line + 2);

	if (query_params.size() > 0) {
		std::cout << "Query params: " << std::endl;
		for (const auto& param : query_params) {
			std::cout << param.first << " = " << param.second << std::endl;
		}
	}
}

void Request::set_headers_from_request_str(std::string& request) {
	headers = {};
	// Delimits start of request body
	size_t double_eol_split = request.find("\r\n\r\n");
	std::istringstream headers_stream(request.substr(0, double_eol_split));

	std::string line;

	while (std::getline(headers_stream, line)) {
		size_t header_name_value_split = line.find(":");

		// Header name is case-insensitive
		std::string header_name = StringFormatting::to_lower(line.substr(0, header_name_value_split));

		// Remove whitespaces and \t characters from header value
		std::string header_value = StringFormatting::trim(line.substr(header_name_value_split + 1));
		headers.emplace(header_name, header_value);
	}

	// Remove headers from request
	request = request.substr(double_eol_split + 4);

	std::cout << "Headers:" << std::endl;
	for (const auto& it : headers) {
		std::cout << it.first << " = " << it.second << std::endl;
	}
}

void Request::set_body_from_request_str(std::string& request)
{
}

std::string Request::get_body_as_str() {
	return body_str;
}

std::unordered_multimap<std::string, std::string> Request::get_query_params() {
	return query_params;
}

std::unordered_multimap<std::string, std::string> Request::get_headers() {
	return headers;
}
