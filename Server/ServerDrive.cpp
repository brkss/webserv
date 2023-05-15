#include <iostream> 
#include "ServerDrive.hpp"
#include <arpa/inet.h> // inte_aton
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <strings.h>
#include "Network.hpp"

ServerDrive::ServerDrive(const Parse &conf): _config(conf), _fd_max(0) {
	const std::vector<Server>	&servers = this->_config.getServers(); 
	const int 					true_ = 1;
	int							sock_fd;

	FD_ZERO(&(this->_readset));
	FD_ZERO(&(this->_writeset));
	FD_ZERO(&(this->_listenset));

	for (Parse::cv_itereator cit = servers.begin(); cit != servers.end(); cit++)
	{
			sock_fd = Network::CreateSocket();
			setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &true_, sizeof(int));
			Network::BindSocket(sock_fd, cit->getPort(), cit->getAddress() );
			Network::ListenOnSocket(sock_fd);
			addSocketFd(sock_fd);
			FD_SET(sock_fd, &(this->_listenset));
			std::cerr << "Server Listening On " <<  cit->getAddress() << "[" << cit->getPort() << "]" << std::endl;
	}
}

ServerDrive::ServerDrive(const ServerDrive &server) : _config(server._config) {
	(void) server;
}

ServerDrive::~ServerDrive() {
	for(std::vector<int>::iterator it = _server_fds.begin(); it != _server_fds.end(); it++)
		close(*it);
}

void ServerDrive::io_select(fd_set &read_copy, fd_set &write_copy) {
	struct timeval timout;
	int		select_stat;

	timout.tv_sec = 1;
	timout.tv_usec = 0;
	select_stat =  select(this->_fd_max + 1, &read_copy, &write_copy, NULL, &timout);
	if  (select_stat < 0)  {
		throw(ErrorLog("Error: Select failed"));
	}
}

void ServerDrive::addClient(Client client) {

	typedef std::map<int, Client>::value_type pair_t;

	int sock_fd = client.getConnectionFd();
	pair_t pair(sock_fd, client);
	this->_clients.insert(pair);
	addSocketFd(sock_fd);
}

void ServerDrive::addSocketFd(int fd) {
	this->_server_fds.push_back(fd);
	this->_fd_max = std::max(this->_fd_max, fd);
	FD_SET(fd, &(this->_readset)); 
}

void ServerDrive::readRequest(int fd) {
	// temp until Request class is created 
	char buffer[1024];
	size_t bytes_recieved;

	Client &client = getClient(fd) ;
	bzero(buffer, sizeof(buffer));
	bytes_recieved = recv(client.getConnectionFd(), buffer, sizeof(buffer), 0);

	if (bytes_recieved <= 0)  // error on while reading 
	{ 	
		if (bytes_recieved == 0) // Client closed connection
		{     
			Network::closeConnection(fd);
			FD_CLR(fd, &this->_readset);
			return ;
		}
		else 
			throw(ErrorLog("Error: unable to read form sock"));
	}
	std::cout << "Recieved Data: " << buffer   << std::endl;
}

void ServerDrive::readFromSocket(fd_set &read_copy) {
	
	int fd_max = this->_fd_max;
	
	for (int fd = 0; fd <=  fd_max; fd++) 
	{
		if (FD_ISSET(fd, &read_copy))
		{
			 if (FD_ISSET(fd, &this->_listenset)) 
				addClient(Network::acceptConnection(fd)); // new connection 
			 else 
			 {	
			 	readRequest(fd);
			 	// response goes here 
			 	Network::closeConnection(fd);
			 	FD_CLR(fd, &this->_readset);
			 }
		}
	}

}

void ServerDrive::run() {
	fd_set read_copy;
	fd_set write_copy;

	std::cerr << "Server Running ..." << std::endl;
	while (1) {

		read_copy = this->_readset;
		write_copy = this->_writeset;
		io_select(read_copy, write_copy);
		readFromSocket(read_copy);
	}

}

Client &ServerDrive::getClient(int fd) {
	clinetiterator_t it = this->_clients.find(fd);

	if (it != this->_clients.end())
		return (it->second);
	else 
		throw(ErrorLog("BUG: Potential   Server  error"));
}
