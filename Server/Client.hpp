#ifndef CLIENT_HPP
# define CLIENT_HPP 

#include <netinet/in.h> // sockaddr_in
#include "HttpRequest.hpp"
#include <time.h>

#ifndef BUFFER_SIZE 
# define BUFFER_SIZE  1024
#endif 

class Client {

	private:
		int 				_server_fd;		 // server socket 
		int 				_connection_fd; // data socket 
		time_t 				_client_request_timout; // timout in secons 
		socklen_t 			_address_len;
		struct sockaddr_in  _client_address;

		HttpRequest			_request;

	public:
		static char 		_buffer[BUFFER_SIZE]; // SHARED BUFFER

		Client();
		~Client();
		Client(int connection_fd);
		Client(const Client &client);
		Client &operator=(const Client &client);

	public:
		struct sockaddr_in*		getClientAddress() ;
		socklen_t*				getAddressLen() ;
		void 					setConnectionFd(int fd);
		int						getServerFd() const;
		int						getConnectionFd() const ;
		time_t					getClientRequestTimeout() const;

		void 					saveRequestData(size_t nb_bytes);
		HttpRequest 			&getRequest();
		void					setRequestTimout(time_t secs);
};



#endif /* CLIENT_HPP */
