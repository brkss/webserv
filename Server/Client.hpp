#ifndef CLIENT_HPP
# define CLIENT_HPP 

#include <netinet/in.h> // sockaddr_in
#include "HttpRequest.hpp"
#include "../config/ConfigParse/inc/server.hpp"
#include "ErrorHandler.hpp"
#include "../Response/Response.hpp"
#include <time.h>
#include <stdlib.h>

#ifndef BUFFER_SIZE 
# define BUFFER_SIZE   0xFFFF
#endif 

class Client {

	private:
		int 				_server_fd;			 	// server socket 
		int 				_connection_fd; 		// data socket 
		time_t 				_client_request_timout; // timout in secons 
		short				_request_status;		// 
		socklen_t 			_address_len;
		struct sockaddr_in  _client_address;

		HttpRequest			_request;				// client request 
		Server				_server;				// server should iterpret the request
		char *				_response;
		char *				_response_cpy;
		size_t				_response_size;
		Response			_responseObj;
		bool				_response_ready;


	public:
		static char 		_buffer[BUFFER_SIZE]; 	// SHARED BUFFER

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
		short					getRequestStatus() const;
		const Server&			getServer() const;
		void 					saveRequestData(size_t nb_bytes);
		HttpRequest 			&getRequest();
		void					setRequestTimout(time_t secs);
		void					setRequestStatus(short error_num);
		void					setServer(const Server &server);

		char *					getResponse() const;
		size_t					getResponseSize() const ;
		Response 				&getResponseObj() ;
		void					setResponse(char *response, size_t size);
		void 					setResponseObj(const Response &resp);
		bool					ResponseReady() const;


};



#endif /* CLIENT_HPP */
