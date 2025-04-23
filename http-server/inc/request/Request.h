#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sstream>

#include "utils/StringFormatting.h"
#include "utils/HttpMethods.h"

class Request {
	protected:
		std::string uri;
		Http::Method method;
		std::string body_str;
		std::unordered_multimap<std::string, std::string> query_params;
		std::unordered_multimap<std::string, std::string> headers;
		void set_method_from_request_str(std::string& request);
		void set_uri_and_query_params_from_request_str(std::string& request);
		void set_headers_from_request_str(std::string& request);
		void set_body_from_request_str(std::string& request);
	public:
		Request(std::string request_str);
		std::string get_uri();
		Http::Method get_method() const;
		std::string get_body_as_str();
		std::unordered_multimap<std::string, std::string> get_query_params();
		std::unordered_multimap<std::string, std::string> get_headers();
};
