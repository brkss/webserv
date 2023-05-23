
#include "Server.hpp"
#include <cstring>
#include <netinet/in.h>
#include <sys/_types/_socklen_t.h>
#include "../Response/Response.hpp"
#include "../Handler/Handler.hpp"
#include <vector>


size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

// helper function parse request and get path 
std::string get_path_from_request(char buffer[30000]){
	std::string req(buffer, 30000);

	std::vector<std::string> values;
	split(req, values, ' ');
	return (values[1]);
}


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
	//std::cout << "buffer : " << buffer << "\n";
}

void Server::responder(){
	
	std::string _root = "/Users/brkss/Developer/1337/webserv/Response/test-files";
	std::string _path = _root + get_path_from_request(buffer);

	Handler handler(_path);
	Response response(handler.getBody(), handler.getType(), handler.getSize());

	std::string resp = response.generateResponse();
	const char *r = resp.data();
	
	
	int status = send(new_sock, r, resp.size(), 0);
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

