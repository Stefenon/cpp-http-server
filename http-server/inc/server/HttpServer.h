#pragma once
#include <iostream>
#include <vector>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "router/HttpRouter.h"
#include "status/StatusCodes.h"

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

		void send_response(std::string response_body, HttpStatusCode status_code, std::string content_type = "application/json") const;

	public:
		HttpServer(int new_port=5000, int new_connection_queue_size=2);

		void start();
		void set_router(HttpRouter router);
};
