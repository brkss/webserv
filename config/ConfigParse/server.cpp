#include "server.hpp"
#include "location.hpp"

Server::Server() : Http() {

}

Server::Server(const Server& server) : Http(server) {
	*this = server;
}

Server &Server::operator=(const Server & server) {

	if (this != &server)
	{
		Http::operator=(server);
		this->_address = server._address; 
		this->_port = server._port;
		this->_server_name = server._server_name;
		//this->_locations = server._locations;
		this->_return_URL = server._return_URL;
		this->_return_code = server._return_code;
	}
	return (*this);
}

Server::~Server() {

}

const std::string			&Server::getAddress() const {
	return (this->_address);
}

const short 				&Server::getPort() const {
	return (this->_port);
}

const std::string 			&Server::getServerName() const {
	return (this->_server_name);
}

const std::vector<Location> &Server::getLocations() const {
	return (this->_locations);
}

const std::string			&Server::getReturnURL() const {
	return (this->_return_URL);
}

const short					&Server::getReturnCode() const {
	return (this->_return_code);
}




