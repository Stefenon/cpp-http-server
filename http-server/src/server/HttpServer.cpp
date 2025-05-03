#include "server/HttpServer.h"
#include "utils/StringFormatting.h"

HttpServer::HttpServer(int new_port, int new_connection_queue_size, int new_buffer_size)
{
	connection_queue_size = new_connection_queue_size;
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

static Response get_sample_html_response(std::unordered_multimap<std::string, std::string> tags) {
	HtmlDocument doc = HtmlDocument();
	doc.head.add_child_element(HtmlElement("title", "Title 123"));
	doc.body.add_child_element(HtmlElement("h1", "Header 345", { {"class", "test"} }));

	HtmlElement new_div = HtmlElement("div");
	new_div.set_text("One more test");

	for (const auto& query_param : tags) {
		HtmlElement header_1 = HtmlElement("h1", query_param.first, {{"class", "header_1"}});
		HtmlElement header_2 = HtmlElement("h2", query_param.second, { {"class", "header_2"} });
		header_2.set_tail("Text after header");
		header_2.set_attribute("new_attr", "abc");
		new_div.add_child_element(header_1);
		new_div.add_child_element(header_2);
	}

	doc.body.add_child_element(new_div);

	return Response(doc.to_string(), HttpStatusCode::HTTP_202_ACCEPTED, "text/html");
}

static JsonResponse get_sample_json_response(const std::string& json_str="") {
	nlohmann::json body;

	if (!json_str.empty()) {
		body = nlohmann::json::parse(json_str);
	}
	else {
		body["numeric_field"] = 123;
		body["text_field"] = 123;
		nlohmann::json object_field;
		object_field["field_a"] = 1;
		object_field["field_b"] = "B";
		body["object_field"] = object_field;
		std::vector<int> array_field = { 1, 2, 3, 4 };
		body["array_field"] = array_field;
	}

	return JsonResponse(body, HttpStatusCode::HTTP_200_OK);
}

void HttpServer::start()
{
	int success = listen(server_fd, connection_queue_size);

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
			Request request(client_fd, buffer_size);
			std::string uri = request.get_uri();

			if (uri == "/html") {
				send_response(get_sample_html_response(request.get_query_params()));
			}
			else if (uri == "/json") {
				send_response(get_sample_json_response(request.get_body_as_str()));
			}
			else {
				send_response(Response(HttpStatusCode::HTTP_204_NO_CONTENT));
			}

			close(client_fd);
		}
	}
}

void HttpServer::set_router(HttpRouter new_router)
{
	router = new_router;
}
