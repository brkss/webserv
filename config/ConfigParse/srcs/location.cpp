#include "../inc/location.hpp"

Location::Location() : Server() {

}

Location::Location(const Location &location) : Server(location) {
	*this = location;
}

Location &Location::operator=(const Location& location) 
{
	if (this != &location)
	{
		Server::operator=(location);
		this->_endpoint = location._endpoint;
		this->_allowed_methods = location._allowed_methods;
	}
	return (*this);

}

Location::~Location() {

}

Location::Location(const Server &server_conf) : Server(server_conf) {

}
void Location::setEndpoint(const std::string &endpoint) {
	this->_endpoint = endpoint;
}

const std::string &Location::getEndpoint() const {
	return (this->_endpoint);
}

void Location::addMethod(const std::string &method) {
	this->_allowed_methods.push_back(method);
}
const std::vector<std::string> &Location::getAllowedMethods() const {
	return (this->_allowed_methods);

}
