#include "Client.hpp"
#include <strings.h>

Client::Client() {

}

Client::Client(int connection_fd) : _server_fd(connection_fd) {
	bzero(&this->_client_address, sizeof(this->_client_address));
	this->_address_len = 0;
}

Client::~Client() {

}

Client::Client(const Client &client) {
	*this = client;
}

Client &Client::operator=(const Client &client) {
	this->_server_fd = client._server_fd;
	this->_connection_fd = client._connection_fd;
	this->_address_len = client._address_len;
	return (*this);
}

int Client::getServerFd() const {
	return (this->_server_fd);
}

int Client::getConnectionFd() const {
	return (this->_connection_fd);
}

struct sockaddr_in *Client::getClientAddress()  {
	return (&this->_client_address);
}

void Client::setConnectionFd(int fd) {
	this->_connection_fd = fd;
}

socklen_t  *Client::getAddressLen()   {
	return (&this->_address_len);
}
