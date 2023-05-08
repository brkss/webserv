



#include "ListeningSocket.hpp"


int main() {

	std::cout << "Listening Socket !\n";
	ListeningSocket sk(AF_INET, SOCK_STREAM, 0, 88, INADDR_ANY, 10);

	return (0);
}
