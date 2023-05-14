
#include "Server.hpp"
#include <cstring>
#include <netinet/in.h>
#include <sys/_types/_socklen_t.h>
#include "../Response/Response.hpp"

Server::Server(int domain, int type, int protocol, int port, unsigned long interface, int bklg){

	sock = new ListeningSocket(domain, type, protocol, port, interface, bklg);

}

void Server::accepter(){
	struct sockaddr_in address = this->sock->get_address();  
	int addrlen = sizeof(address);
	new_sock = accept(sock->get_sd(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
	read(new_sock, buffer, 30000);
}

void Server::handler(){
	std::cout << "buffer : " << buffer << "\n";
}

void Server::responder(){
	Response response("/Users/brkss/Developer/1337/webserv/Response/test.html");

	std::string resp = response.generateResponse();
	const char *r = resp.data();
	//std::cout << resp.length() ;
	
	//char *hello = "";
	int status = write(new_sock, r, std::strlen(r));
	if (status == -1){
		perror("write failed : ");
		exit(EXIT_FAILURE);
	}
	close(new_sock);
}

void Server::launch(){

	while(1){
		std::cout << "[--] : waiting on 127.0.0.1:88" << std::endl;
		this->accepter();
		this->handler();
		this->responder();
	}
}

Server::~Server(){
	delete sock;
}

