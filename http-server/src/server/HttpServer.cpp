#include "server/HttpServer.h"
#include "html/HtmlDocument.h"
#include "json/json.hpp"

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

void HttpServer::send_response(std::string response_body, HttpStatusCode status_code, std::string content_type) const {
	std::string response = "HTTP/1.1 " +  HttpStatus::get_status_line(status_code) + "\r\n";
	response += "Content-Type: " + content_type + "\r\n";
	response += "Content-Length: " + std::to_string(response_body.length()) + "\r\n";

	response += "\r\n" + response_body;

	write(client_fd, response.c_str(), response.length());
}

static std::string get_sample_html_response() {
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

	return doc.to_string();
}

static std::string get_sample_json_response() {
	nlohmann::json response;

	response["numeric_field"] = 123;
	response["text_field"] = 123;
	nlohmann::json object_field;
	object_field["field_a"] = 1;
	object_field["field_b"] = "B";
	response["object_field"] = object_field;
	std::vector<int> array_field = { 1, 2, 3, 4 };
	response["array_field"] = array_field;

	return response.dump();
}

void HttpServer::start()
{
	int success = listen(server_fd, connection_queue_size);
	char buffer[256];

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
			ssize_t n = read(client_fd, buffer, 255);

			if (n < 0) {
				throw std::runtime_error("Error reading message into buffer: " + std::string(strerror(errno)));
			}
			else {
				write(1, buffer, n);

				const std::string response_type = "json";
				std::string content_type;
				std::string body;

				if (response_type == "html") {
					content_type = "text/html";

					body = get_sample_html_response();
				}
				else {
					content_type = "application/json";

					body = get_sample_json_response();
				}

				send_response(body, HttpStatusCode::HTTP_200_OK, content_type);
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
