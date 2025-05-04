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
#include <sstream>

#include "router/HttpRouter.h"
#include "status/StatusCodes.h"
#include "html/HtmlDocument.h"
#include "response/Response.h"
#include "response/JsonResponse.h"
#include "json/json.hpp"
#include "request/Request.h"
#include "exceptions/BadRequestException.h"

class HttpServer {
	private:
		int buffer_size;
		int connection_queue_size;
		int port;
		HttpRouter router;

		// Server socket variables
		int server_fd;
		sockaddr_in server_address;

		// Client socket variables
		int client_fd;
		sockaddr_in client_address;

		void send_response(const Response& response) const;

	public:
		HttpServer(int new_port=5000, int new_connection_queue_size=2, int new_buffer_size=200);

		void start();
		void set_router(HttpRouter router);
};

