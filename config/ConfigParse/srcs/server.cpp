#include "../inc/server.hpp"
#include "../inc/location.hpp"
#include "../inc/defaultConfig.hpp"
#include "../../../Server/ErrorHandler.hpp"

Server::Server() :	Http(),
					_address(DefaulConfig::ADDRESS),
					_port(DefaulConfig::PORT),
					_server_name(DefaulConfig::SERVER_NAME) {

}

Server::Server(const Server& server) : Http(server) {
	*this = server;
}

Server::Server(const Http& http) :Http(http),
					_address(DefaulConfig::ADDRESS),
					_port(DefaulConfig::PORT),
					_server_name(DefaulConfig::SERVER_NAME)  {

	//this->_erro_log = http.getErroLog();
	//this->_root = http.getRoot();
	//this->_index = http.getIndex();
	//this->_autoindex = http.getAutoIndex();
	//this->_client_max_body_size = http.getClientMaxBodySize();
}

Server &Server::operator=(const Server & server) {

	if (this != &server)
	{
		Http::operator=(server);
		this->_address = server._address; 
		this->_port = server._port;
		this->_server_name = server._server_name;
		this->_locations = server._locations;
		this->_return_URL = server._return_URL;
		this->_return_code = server._return_code;
	}
	return (*this);
}

Server::~Server() {

}

bool	Server::isValidServer() const {

	bool ret = true;
	if (this->_server_name == "__REQUIRED") {
		std::cerr << "[server_name] __REQUIRED" << std::endl;
		ret = false;
	}
	if (this->_address == "__REQUIRED") {
		std::cerr << "[listen address] __REQUIRED" << std::endl;
		ret = false;
	}
	if (this->_port == REQUIRED) {
		std::cerr << "[listen port] __REQUIRED" << std::endl;
		ret = false;
	}
	return (ret);
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

void	Server::setAddress(const std::string &address) {
	this->_address = address;
}
void 	Server::setPort(short server_port) {
	this->_port =  server_port;
}
void 	Server::setServerName(const std::string &server_name) {
	this->_server_name = server_name;
}

void	Server::addLocation(const Location &location) {
	this->_locations.push_back(location);
}
void 	Server::setReturnCode(short code) {

	this->_return_code = code;
}

void 	Server::setReturnURL(const std::string &url) {
	this->_return_URL = url;
}




