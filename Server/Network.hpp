#ifndef NETWORK_HPP
# define NETWORK_HPP 
#include <iostream>
#include <arpa/inet.h> // inte_aton
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/ip.h> // has definition of SO_RCVBUF; 


namespace Network
{

	int  CreateSocket()  {
		int sock_fd;
		sock_fd = socket(PF_INET, SOCK_STREAM, 0); 
		if (sock_fd < 0) {
			ConsoleLog::Error("Failed to create Socket !!!");
			throw(ErrorLog("could not create socket"));
		}
		return (sock_fd);
	}
	
	void BindSocket(int sock_fd, short port, const std::string &address)  {
		int bind_stat;
		int set = 1;
		struct sockaddr_in sock_addr;
	
		bzero(&sock_addr, sizeof(sock_addr));
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_port = htons(port);
		if (inet_aton(address.c_str(), &sock_addr.sin_addr) == 0) {  //see man inet_aton 
			ConsoleLog::Error("[::0] Failed to bind On address");
			throw(ErrorLog("Error: invalid address"));
		}
		//	sock_addr.sin_addr.s_addr = inet_addr(address.c_str());
		bind_stat = bind(sock_fd, (struct sockaddr *) &sock_addr, sizeof(sockaddr)); 
		if (bind_stat == -1) {
			throw(ErrorLog("failed to bind socket"));
		}

		setsockopt(sock_fd, SOL_SOCKET, SO_NOSIGPIPE, &set, sizeof(int));

	}
	
	
	void ListenOnSocket(int sock_fd)  { 
		int listen_stat;
	
		listen_stat = listen(sock_fd, 10);
		if (listen_stat == -1) {
			throw(ErrorLog("Error: failed to liten on socket"));
		}
	}
	
	Client acceptConnection(int sock_fd) {
		int connection_fd;
	
		Client new_client(sock_fd) ;
		connection_fd = accept(sock_fd, (struct sockaddr *) new_client.getClientAddress(), new_client.getAddressLen());
		if (connection_fd == -1) {
			throw(ErrorLog("Error: Failed to accept connection"));
		}
		new_client.setConnectionFd(connection_fd);
		ConsoleLog::Debug("New client connection ACCEPT ok!");
		std::cout << "New connection accepted with  fd : " << connection_fd << std::endl;
		return (new_client);
	}
	
	void closeConnection(int fd) {
		close(fd);
		//if (close(fd) == -1)
		//	throw(ErrorLog("Error:  Unalle to close connection"));
	}
	size_t getSocketBufferSize(int socket_fd, int io_type) {

		size_t size;
		socklen_t type_size = sizeof(size);
		getsockopt(socket_fd,SOL_SOCKET,io_type,(void *)&size, &type_size); 
		return (size);
}

}

#endif /* NETWORK_HPP */
