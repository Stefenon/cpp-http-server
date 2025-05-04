#include "inc/server/HttpServer.h"
#include "inc/html/HtmlDocument.h"

using namespace std;

constexpr int PORT = 5000;
constexpr int CONNECTION_QUEUE_SIZE = 2;
constexpr int BUFFER_SIZE = 1024;

int main()
{
	cout << "Create HTTP server instance" << endl;
	HttpServer server = HttpServer(PORT, CONNECTION_QUEUE_SIZE, BUFFER_SIZE);
	cout << "Start listening" << endl;
	server.start();

	return 0;
}
