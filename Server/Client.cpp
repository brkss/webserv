#include "Client.hpp"
#include <strings.h>


// STATIC SHARED BUFFER 
char Client::_buffer[1024];

Client::Client() {
	
}

Client::Client(int connection_fd) : _server_fd(connection_fd) {
	bzero(&this->_client_address, sizeof(this->_client_address));
	this->_address_len = 0;
	this->_client_request_timout = time(NULL);
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
	this->_request = client._request;
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

void 	Client::saveRequestData(size_t nb_bytes) {
	std::string str_bytes(this->_buffer, nb_bytes);
	this->_request.addRequestData(str_bytes); // CAN'T RELY ON INPLICIT CONVERSION 
}

HttpRequest 			&Client::getRequest() {
	return (this->_request);
}

void	Client::setRequestTimout(time_t  secs) {
	this->_client_request_timout = secs;
}

time_t					Client::getClientRequestTimeout() const {
	return (this->_client_request_timout);
}
