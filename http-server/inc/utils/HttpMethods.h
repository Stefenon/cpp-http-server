#include <string>

namespace Http {
	enum Method {
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

	std::string get_string_from_method(Http::Method method);
	Http::Method get_method_from_string(std::string method_string);
}