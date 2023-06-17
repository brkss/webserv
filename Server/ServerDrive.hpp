#ifndef SERVERDRIVE_HPP
# define SERVERDRIVE_HPP 

#include  "../config/ConfigParse/inc/parse.hpp"
#include  "../config/ConfigParse/inc/server.hpp"
#include "Client.hpp"
#include "../Response/Response.hpp"
#include "../Handler/Handler.hpp"
#include <map>

class   ServerDrive {

	private:
		Parse&				_config;
		const std::vector<Server>&	_virtual_servers;

		fd_set						_readset;
		fd_set 		   				_writeset;
		fd_set 		   			 	_listenset;

		std::vector<int> 			_server_fds;
		std::map<int, Client> 		_clients;

		int			   				_fd_max;
		size_t 						_client_timeout;

	public:
		static const std::string HEADER_DELIM;
		static const std::string CRLF;

	public:
		typedef std::map<int, Client>::iterator clinetiterator_t ;
		ServerDrive(Parse &conf);
		ServerDrive(const ServerDrive &server);
		~ServerDrive();

		void addClient(Client client);
		void addSocketFd(int fd);
		void run();

		Client &getClient(int fd);

		void io_select(fd_set &read_copy, fd_set &write_copy);
		void eventHandler(fd_set &read_copy, fd_set &write_copy);
		void readRequest(int fd);
		bool unchunkBody(HttpRequest &request);

		void CheckRequestStatus(Client &client);
		void CloseConnection(int fd);
		void checkClientTimout(int fd);

		void SendResponse(Client &client);

		void getHeader(HttpRequest &request);
		bool getBody(HttpRequest &client_request);
		bool ClientError(int fd);
		const Server &getServerByName(const std::string &host_name);
		const Server &getServerByFd(int fd);

		void log( Client &client); // Logs request line and status code

};

#endif /* SERVERDRIVE_HPP */
