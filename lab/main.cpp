



#include "Server.hpp"

int main() {

	Server server(AF_INET, SOCK_STREAM, 0, 89, INADDR_ANY, 10);
	std::cout << "Starting Server !\n";

	server.launch();
	return (0);
}
