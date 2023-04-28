#include  "Server.hpp"

Server::Server() {}
Server::~Server() {}


Server::Server(const Server& server_)
{
	*this = server_;
}

Server& Server::operator=(const Server  &server_)
{
	if (this != &server_)
	{
		this->_server_names = server_._server_names;
		this->_error_page = server_._error_page;
		this->_listen_port = server_._listen_port;
		this->_max_body_size = server_._max_body_size;
	}
	return (*this);
}

Location::Location(): _enable_list(true)
{
	this->_allowed_methods.push_back("GET");
} 

Location::~Location() {}

Location::Location(const Location &location_)
{
	*this =  location_;
}

Location &Location::operator=(const Location &location_)
{
	if (this != &location_)
	{
		this->_allowed_methods  = location_._allowed_methods;
		this->_route  = location_._route;
		this->_server_root = location_._server_root;
		this->_default_index = location_._default_index;
		this->_enable_list = location_._enable_list;
	}
	return (*this);
}
