#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
	

//int	socket(int domain, int type, int protocol);
//int bind(int socket, const struct sockaddr *address, socklen_t address_len);
//int	listen(int socket, int backlog);
//int acccept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);


int main(void)
{
	struct sockaddr_in socket_add;
	int socket_fd, new_socket_fd;
	char sock_buff[1024];
	int  sock_buff_size; 

	sock_buff_size = 1024;
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		perror(NULL);
		return (-1);
	}

	socket_add.sin_family = AF_INET;
	socket_add.sin_port = htons(8080);
	socket_add.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(socket_fd, (struct sockaddr *) &socket_add, sizeof(socket_add)))
	{
		perror(NULL);
		return (-2);	
	}
	
	if (listen(socket_fd, 2) == -1)
	{
		perror(NULL);
		return (-3);	
	}
		
	while (1)
	{
		std::cout << "--------------------------------"   << std::endl;
		new_socket_fd = accept(socket_fd, NULL, NULL);
		if (new_socket_fd < 0)
			perror(NULL);

		int bytes_read = read(new_socket_fd, sock_buff, sock_buff_size);
		sock_buff[bytes_read] = 0; 
		std::cout << sock_buff << std::endl; 
		close(new_socket_fd);
	}
	close(socket_fd);
}
