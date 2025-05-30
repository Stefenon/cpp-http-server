#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "utils/StringFormatting.h"
#include "router/HttpRouter.h"
#include "status/StatusCodes.h"
#include "html/HtmlDocument.h"
#include "response/Response.h"
#include "response/JsonResponse.h"
#include "json/json.hpp"
#include "request/Request.h"
#include "exceptions/CustomExceptions.h"

class HttpServer
{
private:
	int buffer_size;
	int connection_queue_size;

	std::queue<int> connection_queue;
	std::vector<std::thread> thread_pool;
	std::mutex queue_mutex;
	std::condition_variable cv;

	int port;
	HttpRouter router;

	// Server socket variables
	int server_fd = -1;
	sockaddr_in server_address;

	void send_response(const Response &response, const int &client_fd) const;

	void connection_thread();

	void handle_connection(const int client_fd);

public:
	HttpServer(int new_port = 5000, int new_connection_queue_size = 100, int new_buffer_size = 200);

	~HttpServer();
	HttpServer(HttpServer &&other) noexcept;
	HttpServer &operator=(HttpServer &&other) noexcept;

	// HttpServer instances shouldn't be copied
	HttpServer(const HttpServer &) = delete;
	HttpServer &operator=(const HttpServer &) = delete;

	void start();
	void set_router(HttpRouter router);
};
