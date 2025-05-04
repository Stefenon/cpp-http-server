#include "utils/HttpMethods.h"

std::string Http::get_string_from_method(Http::Method method) {
	switch (method) {
		case Http::Method::GET:
			return "GET";
		case Http::Method::POST:
			return "POST";
		case Http::Method::PUT:
			return "PUT";
		case Http::Method::PATCH:
			return "PATCH";
		case Http::Method::DELETE:
			return "DELETE";
		case Http::Method::HEAD:
			return "HEAD";
		case Http::Method::OPTIONS:
			return "OPTIONS";
		case Http::Method::CONNECT:
			return "CONNECT";
		case Http::Method::TRACE:
			return "TRACE";
		case Http::Method::INVALID_METHOD:
			return "INVALID_METHOD";
	}

	return "INVALID_METHOD";
}

Http::Method Http::get_method_from_string(std::string method_string) {
	if (method_string == "GET")
		return Http::Method::GET;
	else if (method_string == "POST")
		return Http::Method::POST;
	else if (method_string == "PUT")
		return Http::Method::PUT;
	else if (method_string == "PATCH")
		return Http::Method::PATCH;
	else if (method_string == "DELETE")
		return Http::Method::DELETE;
	else if (method_string == "HEAD")
		return Http::Method::HEAD;
	else if (method_string == "OPTIONS")
		return Http::Method::OPTIONS;
	else if (method_string == "CONNECT")
		return Http::Method::CONNECT;
	else if (method_string == "TRACE")
		return Http::Method::TRACE;
	else
		return Http::Method::INVALID_METHOD;
}
