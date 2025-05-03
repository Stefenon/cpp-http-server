#include "request/Request.h"

Request::Request(int client_fd, int buffer_size)
{
	fd = client_fd;
	char buffer[buffer_size];
	std::string request_str;

	ssize_t n;

	while (1) {
		n = recv(client_fd, buffer, (size_t)buffer_size - 1, 0);
		if (n <= 0) {
			if (errno == EAGAIN) {
				break;
			}
			throw std::runtime_error("Error accepting message: " + std::string(strerror(errno)));
		}

		buffer[n] = '\0';

		request_str.append(buffer);
		if (uri.empty()) {
			size_t request_line_end_index = set_request_line_attributes(request_str);
			if (request_line_end_index != std::string::npos) {
				request_str = request_str.substr(request_line_end_index + 2);
			}
		}

		if (headers.size() == 0) {
			size_t headers_end_index = set_headers_from_request_str(request_str);
			if (headers_end_index != std::string::npos) {
				request_str = request_str.substr(headers_end_index + 4);

				if (get_content_length() == -1) {
					break;
				}
			}
		}
		else if (request_str.size() > 0) {
			body_str.append(request_str);
			request_str.clear();
			if (body_str.size() == get_content_length()) {
				break;
			}
		}

		memset(buffer, 0, sizeof(buffer));
	}

	//print_attributes();
}

void Request::print_attributes() {
	std::cout << "Method: " << get_string_from_method(method) << std::endl;

	std::cout << "URI: " << uri << std::endl;

	if (query_params.size() > 0) {
		std::cout << "Query params: " << std::endl;
		for (const auto& param : query_params) {
			std::cout << param.first << " = " << param.second << std::endl;
		}
	}

	std::cout << "Headers:" << std::endl;
	for (const auto& header : headers) {
		std::cout << header.first << " = " << header.second << std::endl;
	}

	std::cout << "Body: " << body_str << std::endl;
}

long int Request::get_content_length()
{
	std::unordered_map<std::string, std::string>::const_iterator got = headers.find("content-length");
	if (got == headers.end()) {
		return -1;
	}
	else {
		return std::stol(got->second);
	}
}

std::string Request::get_uri() {
	return uri;
}

Http::Method Request::get_method() const {
	return method;
}

size_t Request::set_request_line_attributes(const std::string& request_str)
{
	size_t request_line_end_idx = request_str.find(CRLF);
	
	if (request_line_end_idx != std::string::npos) {
		std::string request_line = request_str.substr(0, request_line_end_idx);
		size_t method_name_split = request_line.find(" ");
		std::string method_string = request_line.substr(0, method_name_split);
		request_line = request_line.substr(method_name_split + 1);

		method = Http::get_method_from_string(method_string);

		size_t uri_split = request_line.find(" ");
		std::string uri_string = request_line.substr(0, uri_split);
		size_t query_split = uri_string.find("?");
		std::string query_string = uri_string.substr(query_split + 1);
		uri = uri_string.substr(0, query_split);

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
	}

	return request_line_end_idx;
}

size_t Request::set_headers_from_request_str(const std::string& request) {
	headers = {};
	// Delimits start of request body
	size_t headers_end_index = request.find(CRLF + CRLF);
	if (headers_end_index != std::string::npos) {
		std::istringstream headers_stream(request.substr(0, headers_end_index));

		std::string line;

		while (std::getline(headers_stream, line)) {
			size_t header_name_value_split = line.find(":");

			// Header name is case-insensitive
			std::string header_name = StringFormatting::to_lower(line.substr(0, header_name_value_split));

			// Remove whitespaces and \t characters from header value
			std::string header_value = StringFormatting::trim(line.substr(header_name_value_split + 1));
			headers.emplace(header_name, header_value);
		}
	}

	return headers_end_index;
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
