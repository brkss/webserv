#ifndef  SERVER_HPP
#define  SERVER_HPP

#include <vector>
#include <string>

class Location
{

	public:
		std::vector<std::string> 	_allowed_methods;
		std::string 				_route;
		std::string 				_server_root;
		std::string 				_default_index;
		bool						_enable_list;

	public:
		Location();
		Location(const Location& location_);
		Location &operator=(const Location &location_);
		~Location();
};

class Server
{
	public:
		std::vector<Location>		_locations;
		std::vector<std::string>	_server_names;
		std::string 				_error_page;
		short						_listen_port;
		int							_max_body_size;

	public:
		Server();
		Server(const Server& server_);
		Server  &operator=(const Server &server_);
		~Server();
};

#endif /* SERVER_HPP */
