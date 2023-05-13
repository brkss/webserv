#include <iostream> 
#include "ServerDrive.hpp"
#include <arpa/inet.h> // inte_aton
 #include <sys/socket.h>
#include <exception>
#include <unistd.h>
#include <strings.h>

class ErrorLog : public std::exception {
	private:
		const  char *_error;
	public:
		ErrorLog(const char * error) : _error(error) {
		}
		ErrorLog(const ErrorLog &error_log) : _error(error_log._error) {
		}
		const char *what() const throw() {
			return  this->_error;
		}
};

namespace Network {

int  CreateSocket()  {
		int sock_fd;
		sock_fd = socket(PF_INET, SOCK_STREAM, 0); 
		if (sock_fd < 0) {
			throw(ErrorLog("could not create socket"));
		}
		std::cerr << "Socket create OK!" << std::endl;
		return (sock_fd);
	}

void BindSocket(int sock_fd, short port, const std::string &address)  {
	int bind_stat;
	struct sockaddr_in sock_addr;

	bzero(&sock_addr, sizeof(sock_addr));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	if (inet_aton(address.c_str(), &sock_addr.sin_addr) == 0) {  //see man inet_aton 
		throw(ErrorLog("Error: invalid address"));
	}
//	sock_addr.sin_addr.s_addr = inet_addr(address.c_str());
	bind_stat = bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sockaddr)); 
	if (bind_stat == -1) {
		perror(NULL);
		std::cerr << "address: " << address << std::endl;
		throw(ErrorLog("failed to bind socket"));
	}
	std::cerr << "Socket bind OK!" << std::endl; 
}

void ListenOnSocket(int sock_fd)  { 
	int listen_stat;

	listen_stat = listen(sock_fd, 10);
	if (listen_stat == -1) {
		throw(ErrorLog("Error: failed to liten on socket"));
	}
	std::cerr << "Socket listen OK!" << std::endl;
}

Client acceptConnection(int sock_fd) {
	int connection_fd;

	Client new_client(sock_fd) ;

	connection_fd = accept(sock_fd, new_client.getClientAddress(), new_client.getAddressLen());
	if (connection_fd == -1) {
		throw(ErrorLog("Error: Failed to accept connection"));
	}
	new_client.setConnectionFd(connection_fd);
	return (new_client);
}

}

ServerDrive::ServerDrive(const Parse &conf): _config(conf), _fd_max(0) {
	int sock_fd;
	const std::vector<Server> &servers = this->_config.getServers(); 

	FD_ZERO(&(this->_readset));
	FD_ZERO(&(this->_writeset));
	FD_ZERO(&(this->_listenset));

	for (Parse::cv_itereator cit = servers.begin(); cit != servers.end(); cit++) {
			sock_fd = Network::CreateSocket();
			Network::BindSocket(sock_fd, cit->getPort(), cit->getAddress() );
			Network::ListenOnSocket(sock_fd);
			// Socket ready to accept 

			addSocketFd(sock_fd);
			FD_SET(sock_fd, &(this->_listenset));
			
			std::cerr << "Server Listening On " <<  cit->getAddress() << "[" << cit->getPort() << "]" << std::endl;
	}
	std::cerr << this->_fd_max << std::endl;
	
}

void ServerDrive::io_select(fd_set &read_copy, fd_set &write_copy) {
	struct time_val timout;
	int		select_stat;

	timout.tv_sec = 10;
	timout.tv_usec = 0;

	read_copy = this->_readset;
	write_copy = this->_writeset;
	select_stat =  select(this->_fd_max + 1, read_copy, write_copy, NULL, &timout);
	if  (select_stat < 0)  {
		throw(ErrorLog("Error: Select failed"));
	}

}

void ServerDrive::addClient(Client client) {
	int sock_fd = client.getConnectionFd();
	this->_clients.insert(client.getServerFd(), client);
	this->_fd_max = std::max(this->_fd_max, sock_fd);
	addSocketFd(sock_fd);
}

void ServerDrive::readFromSocket(fd_set &read_copy) {
	
	// temp until Request class is created 
	char buffer[1024];
	size_t bytes_recieved;

	int fd_max = this->_fd_max;
	
	for (int fd = 0; fd <  fd_max; fd++) {
		if (FD_ISSET(fd, &read_copy)) { // event on socket 
			if (FD_ISSET(fd, &this->_listenset)) { // new connection 
				addClient(Network::acceptConnection());
			}
			else {	// recieved data on event 
				Client &client = getClient(fd) ;
				bytes_recieved = recv(client.getConnectionFd(), buffer, sizeof(buffer), 0);
				if (bytes_recieved <= 0) { // error on while reading 
					if (bytes_recieved == 0) { // clint closed connection
						// Close client connection
						// Drop client 
						// Close write end of socket 
					}
					else 
						throw(ErrorLog("Error: unable to read form sock"));
				}
			}
			// std::cout buffer 
		}
	}

}

void ServerDrive::run() {
	fd_set read_copy;
	fd_set write_copy;

	while (1) {
		io_select(read_copy, write_copy);	
		// response goes here 
		//
	}

}

ServerDrive::ServerDrive(const ServerDrive &server) : _config(server._config) {
	(void) server;

}
ServerDrive::~ServerDrive() {
	for(std::vector<int>::iterator it = _server_fds.begin(); it != _server_fds.end(); it++)
		close(*it);
}

void ServerDrive::addSocketFd(int fd) {
	this->_server_fds.push_back(fd);
	FD_SET(sock_fd, &(this->_readset));
}

Client &ServerDrive::getClient(int fd) {
	clinetiterator_t it = this->_clients.find(fd);
	if (it != this->_clients.end())
		return (it->second);

	else {
		throw(ErrorLog("BUG: Potential   Server  error"));
		return  (it->second); // surpress compiler error 
	}
}
