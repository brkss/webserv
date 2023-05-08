

#include "Socket.hpp"
#include <netinet/in.h>
#include <sys/socket.h>


SocketWrapper::SocketWrapper(int domain, int type, int protocol, int port, unsigned long interface){

	// define address structure 
	address.sin_family = domain;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = htonl(interface);

	// creating connection 
	sd = socket(domain, type, protocol);
	if (sd < 0){
		perror("Cant Create Socker !");
		exit(EXIT_FAILURE);
	}
	this->connect_to_network();
}

void SocketWrapper::connect_to_network(){
	connection = bind(sd, (struct sockaddr *)&address, sizeof(address));
	if (connection < 0){
		perror("Cant connect to the network !");
		exit(EXIT_FAILURE);
	}
}

struct sockaddr_in SocketWrapper::get_address(){
	return this->address;
}

int SocketWrapper::get_sd(){
	return this->sd;
}

int SocketWrapper::get_connection(){
	return this->connection;
}

