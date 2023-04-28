class ServerSocket
{
	private:
		struct sockaddr_in socket_addr;
		int _bind_port;
		int	_sock_fd;
		int _domain;

	public:
		int	socket(int domain, int type, int protocol);

		int bind(int socket, const struct sockaddr *address, socklen_t address_len);
		int	listen(int socket, int backlog);
		int acccept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len);


		ServerSocket(int domain, int type);
		~ServerSocket();
}

int	ServerSocket::socket(int domain, int type, int protocol)
	: _domain()
{
	

}
