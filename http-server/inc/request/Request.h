#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <cstdint>

#include "utils/StringFormatting.h"
#include "utils/HttpMethods.h"
#include "exceptions/CustomExceptions.h"
#include <response/Response.h>

class Request {
	protected:
		std::string uri;
		Http::Method method;
		std::string body_str;
		std::unordered_multimap<std::string, std::string> query_params;
		std::unordered_map<std::string, std::string> path_params;
		std::unordered_multimap<std::string, std::string> headers;
		long int get_content_length() const;
		size_t set_request_line_attributes(const std::string& request_str);
		size_t set_headers_from_request_str(const std::string& request);

	public:
		Request(int client_fd, int buffer_size);
		bool method_accepts_body() const;
		std::string get_uri() const;
		Http::Method get_method() const;
		std::string get_body_as_str() const;
		std::unordered_multimap<std::string, std::string> get_query_params() const;
		std::unordered_map<std::string, std::string> get_path_params() const;
		void set_path_params(const std::unordered_map<std::string, std::string>& new_path_params);
		std::unordered_multimap<std::string, std::string> get_headers() const;
		void print_attributes() const;
};
