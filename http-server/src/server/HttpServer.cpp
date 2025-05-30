#include "server/HttpServer.h"

void HttpServer::connection_thread()
{
	std::unique_lock<std::mutex> lock(queue_mutex);
	if (!connection_queue.empty())
	{
		int client_fd = connection_queue.front();
		connection_queue.pop();
		lock.unlock();

		handle_connection(client_fd);
	}
}

HttpServer::HttpServer(int new_port, int new_connection_queue_size, int new_buffer_size)
{
	connection_queue_size = new_connection_queue_size;
	for (int i = 0; i < connection_queue_size; i++)
	{
		thread_pool.emplace_back(
				[this]
				{
			while (true) {
				connection_thread();
			} });
	}

	port = new_port;
	buffer_size = new_buffer_size;

	// Create IPv4 socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

	// Accept any incoming messages
	server_address.sin_addr.s_addr = INADDR_ANY;
	// Indicate address family is IP
	server_address.sin_family = AF_INET;
	// Sets port at which server will be available
	server_address.sin_port = htons(port);

	int success = bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address));

	if (success == 0)
	{
		std::cout << "Socket successfully bound to port " << ntohs(server_address.sin_port) << std::endl;
	}
	else
	{
		throw std::runtime_error("Socket binding failed: " + std::string(strerror(errno)));
	}
}

HttpServer::~HttpServer()
{
	if (server_fd != -1)
		close(server_fd);
}

HttpServer::HttpServer(HttpServer &&other) noexcept : buffer_size(other.buffer_size),
																											connection_queue_size(other.connection_queue_size),
																											port(other.port),
																											router(std::move(other.router)),
																											server_fd(other.server_fd),
																											server_address(other.server_address)
{
	other.server_fd = -1;
}

HttpServer &HttpServer::operator=(HttpServer &&other) noexcept
{
	if (this != &other)
	{
		if (server_fd != -1)
			close(server_fd);

		buffer_size = other.buffer_size;
		connection_queue_size = other.connection_queue_size;
		port = other.port;
		router = std::move(other.router);

		server_fd = other.server_fd;
		server_address = other.server_address;

		other.server_fd = -1;
	}
	return *this;
}

void HttpServer::send_response(const Response &response, const int &client_fd) const
{
	std::string response_str = "HTTP/1.1 " + HttpStatus::get_status_line(response.status_code) + "\r\n";
	std::string content_str = response.content_to_string();

	response_str += "Content-Type: " + response.content_type + "\r\n";
	response_str += "Content-Length: " + std::to_string(content_str.length()) + "\r\n";

	response_str += "\r\n" + content_str;

	write(client_fd, response_str.c_str(), response_str.length());
	close(client_fd);
}

void HttpServer::handle_connection(const int client_fd)
{
	try
	{
		Request request(client_fd, buffer_size);

		Http::Method method = request.get_method();
		std::string uri = request.get_uri();
		const auto [endpoint_function, path_params] = router.get_endpoint_function(method, uri);
		request.set_path_params(path_params);

		Response response = endpoint_function(request);
		send_response(response, client_fd);
	}
	catch (const BadRequestException &e)
	{
		nlohmann::json request_body = {{"detail", e.what()}};
		send_response(JsonResponse(request_body, HttpStatusCode::HTTP_400_BAD_REQUEST), client_fd);
	}
	catch (const EndpointNotFoundException &e)
	{
		send_response(Response(HttpStatusCode::HTTP_404_NOT_FOUND), client_fd);
	}
	catch (const MethodNotAllowedException &e)
	{
		nlohmann::json request_body = {{"detail", e.what()}};
		send_response(JsonResponse(request_body, HttpStatusCode::HTTP_405_METHOD_NOT_ALLOWED), client_fd);
	}
	catch (const std::exception &e)
	{
		std::cout << "std::exception caught: " << e.what() << std::endl;
		send_response(Response(HttpStatusCode::HTTP_500_INTERNAL_SERVER_ERROR), client_fd);
	}
	catch (...)
	{
		std::cout << "Unknown exception caught" << std::endl;
		send_response(Response(HttpStatusCode::HTTP_500_INTERNAL_SERVER_ERROR), client_fd);
	};
}

void HttpServer::start()
{
	int success = listen(server_fd, connection_queue_size);

	if (success == 0)
	{
		std::cout << "Socket successfully listening at port " << ntohs(server_address.sin_port) << std::endl;
	}
	else
	{
		throw std::runtime_error("Socket listening failed: " + std::string(strerror(errno)));
	}

	int client_fd;

	sockaddr_in client_address;
	socklen_t client_address_len = sizeof(client_address);

	// Block until message is accepted from client
	while (true)
	{
		client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_address_len);

		if (client_fd != -1)
		{
			connection_queue.push(client_fd);
		}
	}
}

void HttpServer::set_router(HttpRouter new_router)
{
	router = new_router;
}
