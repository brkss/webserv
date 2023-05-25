#ifndef NETWORK_HPP
# define NETWORK_HPP 
#include <iostream>
#include <arpa/inet.h> // inte_aton
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>


namespace Network
{

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
		connection_fd = accept(sock_fd, (struct sockaddr *) new_client.getClientAddress(), new_client.getAddressLen());
		if (connection_fd == -1) {
			throw(ErrorLog("Error: Failed to accept connection"));
		}
		new_client.setConnectionFd(connection_fd);
		std::cerr << "New client connection ACCEPT ok!" << std::endl;
		return (new_client);
	}
	
	void closeConnection(int fd) {
		close(fd);
		//if (close(fd) == -1)
		//	throw(ErrorLog("Error:  Unalle to close connection"));
	}

}

#endif /* NETWORK_HPP */
