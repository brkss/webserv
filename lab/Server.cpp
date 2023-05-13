
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
	Response response;

	std::string resp = response.generateResponse("<html><body><h1>testing response</h1></body></html>");
	const char * r = resp.c_str();
	std::cout << r ;
	
	char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html\nAccept: */*;Date:Sun, 21 Feb 2016 18:26:33 GMT\nServer: ubersnip\nContent-Length:37\nAccept-Encoding:gzip, deflate, sdch\nAccept-Language:en-US,en;q=0.8\nCache-Control:max-age=0\nConnection:keep-alive\nHost:localhost:88\nSet-Cookie:acct=t=t%2flE8KL9jDBOed05o1eMDiINlDfMvLxp&s=CUBOpXFMXmBt4u3pf%2fx5efr5WWkyGT3U; domain=.stackoverflow.com; expires=Sun, 21-Aug-2016 18:26:33 GMT; path=/; HttpOnly\n\n<html><body>Hello World</body></html>";
	//char *hello = "";
	write(new_sock, r, std::strlen(r));
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

