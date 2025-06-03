#include "server/HttpServer.h"

constexpr int KEEP_ALIVE_TIMEOUT = 5;

void HttpServer::connection_thread()
{
	std::unique_lock<std::mutex> lock(queue_mutex);
	if (connection_queue.empty())
	{
		cv.wait(lock, [this]
						{ return !connection_queue.empty(); });
	}

	int client_fd = connection_queue.front();
	connection_queue.pop();
	lock.unlock();

	if (client_fd != -1)
	{
		handle_connection(client_fd);
	}
}

HttpServer::HttpServer(
		int new_port,
		int new_connection_queue_size,
		int new_buffer_size,
		size_t new_max_body_size,
		size_t new_max_headers_size) : port(new_port),
																	 connection_queue_size(new_connection_queue_size),
																	 buffer_size(new_buffer_size),
																	 max_body_size(new_max_body_size),
																	 max_headers_size(new_max_headers_size)
{
	for (int i = 0; i < new_connection_queue_size; i++)
	{
		thread_pool.emplace_back(
				[this]
				{
			while (true) {
				connection_thread();
			} });
	}

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

void HttpServer::send_response(Response &response, const int &client_fd) const
{
	std::string response_str = "HTTP/1.1 " + HttpStatus::get_status_line(response.status_code) + "\r\n";
	std::string content_str = response.content_to_string();

	std::unordered_multimap<std::string, std::string> test;

	response.set_header("content-type", response.content_type);
	response.set_header("content-length", std::to_string(content_str.length()));

	for (const auto &header : response.headers)
	{
		response_str += header.first + ": " + header.second + "\r\n";
	}

	response_str += "\r\n" + content_str;

	write(client_fd, response_str.c_str(), response_str.length());
}

void HttpServer::handle_connection(const int client_fd)
{
	bool keep_alive = true;

	while (keep_alive)
	{
		try
		{
			Request request(client_fd, buffer_size, max_body_size, max_headers_size);

			Http::Method method = request.get_method();
			std::string uri = request.get_uri();
			std::string http_version = request.get_http_version();
			std::unordered_multimap<std::string, std::string> headers = request.get_headers();
			std::string connection_header = "";

			auto it = headers.find("connection");

			if (it != headers.end())
			{
				connection_header = it->second;
			}

			if ((http_version == "HTTP/1.0" && connection_header != "keep-alive") || connection_header == "close")
			{
				keep_alive = false;
			}

			const auto [endpoint_function, path_params] = router.get_endpoint_function(method, uri);
			request.set_path_params(path_params);

			Response response = endpoint_function(request);
			if (keep_alive)
			{
				response.set_header("connection", "keep-alive");
				response.set_header("keep-alive", "timeout=" + std::to_string(KEEP_ALIVE_TIMEOUT));
			}
			else
			{
				response.set_header("connection", "close");
			}

			send_response(response, client_fd);
		}
		catch (const BadRequestException &e)
		{
			nlohmann::json request_body = {{"detail", e.what()}};
			JsonResponse response(request_body, HttpStatusCode::HTTP_400_BAD_REQUEST);
			send_response(response, client_fd);
		}
		catch (const EndpointNotFoundException &e)
		{
			Response response(HttpStatusCode::HTTP_404_NOT_FOUND);
			send_response(response, client_fd);
		}
		catch (const MethodNotAllowedException &e)
		{
			nlohmann::json request_body = {{"detail", e.what()}};
			JsonResponse response(request_body, HttpStatusCode::HTTP_405_METHOD_NOT_ALLOWED);
			send_response(response, client_fd);
		}
		catch (const PayloadTooLargeException &e)
		{
			nlohmann::json request_body = {{"detail", e.what()}};
			JsonResponse response(request_body, HttpStatusCode::HTTP_413_PAYLOAD_TOO_LARGE);
			send_response(response, client_fd);
		}
		catch (const RequestHeaderFieldsTooLargeException &e)
		{
			nlohmann::json request_body = {{"detail", e.what()}};
			JsonResponse response(request_body, HttpStatusCode::HTTP_431_REQUEST_HEADER_FIELDS_TOO_LARGE);
			send_response(response, client_fd);
		}
		catch (const std::exception &e)
		{
			std::cout << "std::exception caught: " << e.what() << std::endl;
			Response response(HttpStatusCode::HTTP_500_INTERNAL_SERVER_ERROR);
			send_response(response, client_fd);
		}
		catch (...)
		{
			std::cout << "Unknown exception caught" << std::endl;
			Response response(HttpStatusCode::HTTP_500_INTERNAL_SERVER_ERROR);
			send_response(response, client_fd);
		};

		struct timeval tv;
		tv.tv_sec = KEEP_ALIVE_TIMEOUT;
		tv.tv_usec = 0;
		setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	}

	close(client_fd);
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
			std::unique_lock lock(queue_mutex);
			connection_queue.push(client_fd);
			lock.unlock();
			cv.notify_one();
		}
	}
}

void HttpServer::set_router(HttpRouter new_router)
{
	router = new_router;
}
