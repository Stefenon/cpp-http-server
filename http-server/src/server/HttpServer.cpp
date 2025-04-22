#include "server/HttpServer.h"
#include "utils/StringFormatting.h"

HttpServer::HttpServer(int new_port, int new_connection_queue_size)
{
	connection_queue_size = new_connection_queue_size;
	port = new_port;

	// Create IPv4 socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);

	// Accept any incoming messages
	server_address.sin_addr.s_addr = INADDR_ANY;
	// Indicate address family is IP
	server_address.sin_family = AF_INET;
	// Sets port at which server will be available
	server_address.sin_port = htons(port);
	
	int success = bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address));

	if (success == 0) {
		std::cout << "Socket successfully bound to port " << ntohs(server_address.sin_port) << std::endl;
	}
	else {
		throw std::runtime_error("Socket binding failed: " + std::string(strerror(errno)));
	}
}

void HttpServer::send_response(const Response& response) const
{
	std::string response_str = "HTTP/1.1 " + HttpStatus::get_status_line(response.status_code) + "\r\n";
	std::string content_str = response.content_to_string();

	response_str += "Content-Type: " + response.content_type + "\r\n";
	response_str += "Content-Length: " + std::to_string(content_str.length()) + "\r\n";

	response_str += "\r\n" + content_str;

	write(client_fd, response_str.c_str(), response_str.length());
}

static Response get_sample_html_response() {
	HtmlDocument doc = HtmlDocument();
	doc.head.add_child_element(HtmlElement("title", "Title 123"));
	doc.body.add_child_element(HtmlElement("h1", "Header 345", { {"class", "test"} }));

	HtmlElement new_div = HtmlElement("div");
	new_div.set_text("One more test");

	HtmlElement header_1 = HtmlElement("h1", "Header 1 inside div", { {"class", "header_1"} });
	HtmlElement header_2 = HtmlElement("h2", "Header 2 inside div", { {"class", "header_2"} });
	header_2.set_tail("Text after header");
	header_2.set_attribute("new_attr", "abc");

	new_div.add_child_element(header_1);
	new_div.add_child_element(header_2);
	new_div.add_child_element(header_1);

	doc.body.add_child_element(new_div);

	return Response(doc.to_string(), HttpStatusCode::HTTP_202_ACCEPTED, "text/html");
}

static JsonResponse get_sample_json_response() {
	nlohmann::json body;

	body["numeric_field"] = 123;
	body["text_field"] = 123;
	nlohmann::json object_field;
	object_field["field_a"] = 1;
	object_field["field_b"] = "B";
	body["object_field"] = object_field;
	std::vector<int> array_field = { 1, 2, 3, 4 };
	body["array_field"] = array_field;

	return JsonResponse(body, HttpStatusCode::HTTP_200_OK);
}

HttpMethod HttpServer::get_method_from_request(std::string& request)
{
	size_t split = request.find(" ");
	std::string method_string = request.substr(0, split);
	request = request.substr(split+1);

	std::cout << "Method: " << method_string << std::endl;

	if (method_string == "GET")
		return HttpMethod::GET;
	else if (method_string == "POST")
		return HttpMethod::POST;
	else if (method_string == "PUT")
		return HttpMethod::PUT;
	else if (method_string == "PATCH")
		return HttpMethod::PATCH;
	else if (method_string == "DELETE")
		return HttpMethod::DELETE;
	else if (method_string == "HEAD")
		return HttpMethod::HEAD;
	else if (method_string == "OPTIONS")
		return HttpMethod::OPTIONS;
	else if (method_string == "CONNECT")
		return HttpMethod::CONNECT;
	else if (method_string == "TRACE")
		return HttpMethod::TRACE;
	else
		return HttpMethod::INVALID_METHOD;
}

static std::pair<std::string, std::vector<std::pair<std::string, std::string>>> get_uri_and_query_params_from_request(std::string& request) {
	size_t uri_split = request.find(" ");
	std::string uri_string = request.substr(0, uri_split);
	size_t query_split = uri_string.find("?");
	std::string query_string = uri_string.substr(query_split + 1);
	uri_string = uri_string.substr(0, query_split);

	std::vector<std::pair<std::string, std::string>> query_params;

	if (query_split != std::string::npos) {
		size_t param_value_split;
		size_t next_param_split;

		do {
			param_value_split = query_string.find("=");
			std::string param_key = query_string.substr(0, param_value_split);
			next_param_split = query_string.find("&");
			std::string param_value = query_string.substr(param_value_split + 1, next_param_split-(param_value_split+1));
			query_params.emplace_back(std::make_pair(param_key, param_value));
			query_string = query_string.substr(next_param_split + 1);
		} while (next_param_split != std::string::npos);
	}

	// Remove request line from request string
	size_t end_of_request_line = request.find("\r\n");
	request = request.substr(end_of_request_line + 2);

	std::cout << "URI: " << uri_string << std::endl;
	if (query_params.size() > 0) {
		std::cout << "Query params: " << std::endl;
		for (const auto& param : query_params) {
			std::cout << param.first << " = " << param.second << std::endl;
		}
	}

	return std::make_pair(uri_string, query_params);
}

static std::unordered_map<std::string, std::string> get_headers_from_request(std::string& request) {
	std::unordered_map<std::string, std::string> headers;
	// Delimits start of request body
	size_t double_eol_split = request.find("\r\n\r\n");
	std::istringstream headers_stream(request.substr(0, double_eol_split));

	std::string line;

	while (std::getline(headers_stream, line)) {
		size_t header_name_value_split = line.find(":");

		// Header name is case-insensitive
		std::string header_name = StringFormatting::to_lower(line.substr(0, header_name_value_split));

		// Remove whitespaces and \t characters from header value
		std::string header_value = StringFormatting::trim(line.substr(header_name_value_split + 1));
		headers[header_name] = header_value;
	}

	// Remove headers from request
	request = request.substr(double_eol_split+4);

	std::cout << "Headers:" << std::endl;
	for (const auto& it : headers) {
		std::cout << it.first << " = " << it.second << std::endl;
	}

	return headers;
}

void HttpServer::process_request(std::string& request) {
	HttpMethod method = get_method_from_request(request);
	auto [uri, query_params] = get_uri_and_query_params_from_request(request);
	std::unordered_map<std::string, std::string> headers = get_headers_from_request(request);

	std::cout << "Remaining request:" << std::endl;
	std::cout << request << std::endl;
}

void HttpServer::start()
{
	int success = listen(server_fd, connection_queue_size);
	char buffer[BUFFER_SIZE];

	if (success == 0) {
		std::cout << "Socket successfully listening at port " << ntohs(server_address.sin_port) << std::endl;
	}
	else {
		throw std::runtime_error("Socket listening failed: " + std::string(strerror(errno)));
	}

	// Block until message is accepted from client
	while (1) {
		socklen_t client_address_len = sizeof(client_address);
		client_fd = accept(server_fd, (struct sockaddr*)&client_address, &client_address_len);

		if (client_fd != -1) {
			ssize_t n = read(client_fd, buffer, (size_t)BUFFER_SIZE - 1);

			if (n < 0) {
				throw std::runtime_error("Error reading message into buffer: " + std::string(strerror(errno)));
			}
			else {
				std::cout << std::endl;
				write(1, buffer, n);
				std::cout << std::endl;
				std::string request_str = std::string(buffer);
				process_request(request_str);

				const std::string response_type = "html";
				std::string content_type;

				if (response_type == "html") {
					send_response(get_sample_html_response());
				}
				else {
					send_response(get_sample_json_response());
				}

				close(client_fd);
			}
		}
		else {
			throw std::runtime_error("Error accepting message: " + std::string(strerror(errno)));
		}
	}
}

void HttpServer::set_router(HttpRouter new_router)
{
	router = new_router;
}
