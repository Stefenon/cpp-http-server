#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "router/HttpRouter.h"
#include "status/StatusCodes.h"
#include "html/HtmlDocument.h"
#include "response/Response.h"
#include "response/JsonResponse.h"
#include "json/json.hpp"

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

class HttpServer {
	private:
		int connection_queue_size;
		int port;
		HttpRouter router;

		// Server socket variables
		int server_fd;
		sockaddr_in server_address;

		// Client socket variables
		int client_fd;
		sockaddr_in client_address;

		HttpMethod get_method_from_request(std::string& request);
		std::pair<std::string, std::vector<std::pair<std::string, std::string>>> get_uri_and_query_params_from_request(std::string& request);
		void process_request(std::string& request);

		void send_response(const Response& response) const;

	public:
		HttpServer(int new_port=5000, int new_connection_queue_size=2);

		void start();
		void set_router(HttpRouter router);
};

