// http-server.cpp : Defines the entry point for the application.

#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "http-server.h"

using namespace std;

constexpr int PORT = 5000;
constexpr int CONNECTION_QUEUE_SIZE = 2;

int main()
{
	// Accepted data buffer
	char buffer[256];

	cout << "Create TCP socket." << endl;
	// Create IPv4 socket
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	cout << "Socket file descriptor: " << server_fd << endl;

	sockaddr_in server_address{};

	bzero((char*)&server_address, sizeof(server_address));

	// Accept any incoming messages
	server_address.sin_addr.s_addr = INADDR_ANY;
	// Indicate address family is IP
	server_address.sin_family = AF_INET;
	// Sets port at which server will be available
	server_address.sin_port = htons(PORT);


	cout << "Bind socket to address" << endl;

	int success = bind(server_fd, (struct sockaddr *) &server_address, sizeof(server_address));

	if (success == 0) {
		cout << "Socket successfully bound to port " << ntohs(server_address.sin_port) << endl;
	}
	else {
		throw runtime_error("Socket binding failed: " + string(strerror(errno)));
	}

	cout << "Listening..." << endl;

	success = listen(server_fd, CONNECTION_QUEUE_SIZE);

	if (success == 0) {
		cout << "Socket successfully listening at port " << ntohs(server_address.sin_port) << endl;
	}
	else {
		throw runtime_error("Socket listening failed: " + string(strerror(errno)));
	}

	sockaddr_in client_address{};
	socklen_t client_address_len = sizeof(client_address);
	
	// Block until message is accepted from client
	int client_fd = accept(server_fd, (struct sockaddr *) &client_address, &client_address_len);
	 
	if (client_fd != -1) {
		cout << "Accepted message from socket " << client_fd << endl;

		bzero(buffer, 256);
		ssize_t n = read(client_fd, buffer, 255);

		if (n < 0) {
			throw runtime_error("Error reading message into buffer: " + string(strerror(errno)));
		}
		else {
			cout << "Buffer read: " << buffer << endl;
			write(1, buffer, n);
			string response = "Got message" + string(buffer) + "\n";

			write(client_fd, &response, sizeof(response));
			close(client_fd);
			close(server_fd);

			return 0;
		}
	}
	else {
		throw runtime_error("Error accepting message: " + string(strerror(errno)));
	}

	return 0;
}
