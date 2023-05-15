#ifndef CLIENT_HPP
# define CLIENT_HPP 

#include <netinet/in.h> // sockaddr_in

class Client {
	private:
		int 				_server_fd; // server socket 
		int 				_connection_fd; // data socket 
		socklen_t 			_address_len;
		struct sockaddr_in _client_address;


	public:
		Client();
		~Client();
		Client(int connection_fd);
		Client(const Client &client);
		Client &operator=(const Client &client);

		int getServerFd() const;
		struct sockaddr_in *getClientAddress() ;
		socklen_t  *getAddressLen() ;
		int getConnectionFd() const ;

		void setConnectionFd(int fd);

};
#endif /* CLIENT_HPP */
