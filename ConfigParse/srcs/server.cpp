#include "../inc/server.hpp"
#include "../inc/location.hpp"
#include "../inc/defaultConfig.hpp"
#include "../../Server/ErrorHandler.hpp"

Server::Server() :	Http(),
					_address(DefaulConfig::ADDRESS),
					_port(DefaulConfig::PORT),
					_server_name(DefaulConfig::SERVER_NAME),
					_upload_store(DefaulConfig::UPLOAD_STORE),
					_allow_upload(DefaulConfig::ALLOW_UPLOADS) {

}

Server::Server(const Server& server) : Http(server) {
	*this = server;
}

Server::Server(const Http& http) :Http(http),
					_address(DefaulConfig::ADDRESS),
					_port(DefaulConfig::PORT),
					_server_name(DefaulConfig::SERVER_NAME),
					_upload_store(DefaulConfig::UPLOAD_STORE),
					_allow_upload(DefaulConfig::ALLOW_UPLOADS) {
		
	this->_error_pages.clear(); // Error  pages are never inherited 
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
		this->_upload_store =  server._upload_store;
		this->_allow_upload = server._allow_upload;
		this->_client_max_body_size = server._client_max_body_size;
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
	if (this->_root == "__REQUIRED") {
		std::cerr << "[" << this->_server_name << "]" <<  "[Webroot] __REQUIRED" << std::endl;
		ret = false;
	}
	if (this->allowUpload())
		validateUplodeStore(*this);
	for (std::vector<Location>::const_iterator cit = this->_locations.begin(); cit != this->_locations.end(); cit++)
		if (cit->allowUpload())
			validateUplodeStore(*cit);
	return (ret);
}

void Server::validateUplodeStore(const Server &scop) const {
	if (scop.getUploadStore().empty())
		return ;
	const std::string &path_name = scop.getRoot() + scop.getUploadStore();
	if (!path_name.empty() and access(path_name.c_str(), F_OK | W_OK) ){ 
		const std::string error = "[Config Error][Server: " + scop.getServerName() + "]" + "[Upload Dir] : " +  path_name ;
		throw(std::invalid_argument(error));
	}
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

const std::string&			Server::getUploadStore() const {
		return (this->_upload_store);
}

int Server::getSocketFd() const {
	return (this->_server_sockfd);
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

void 	Server::setUploadStore(const std::string &path) {	
	this->_allow_upload  = true;
	this->_upload_store = path;
}

void	 Server::setSocketFd(int fd) {
	this->_server_sockfd = fd;
}

bool	Server::allowUpload() const {

	return (this->_allow_upload);
}
