#include "Client.hpp"

Client::Client() {

}

Client::Client(int connection_fd) : _connection_fd(connection_fd) {
	bzero(this->_client_address, sizeof(this->_client_address));
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
	return (this->_server_fd);
}

struct socketaddr_in *Client::getClientAddress() const {
	return (&this->_client_address);
}

void Client::setConnectionFd(int fd) {
	this->_connection_fd = fd;
}

int *Client::getAddressLen() const {
	return (&this->_address_len);
}
