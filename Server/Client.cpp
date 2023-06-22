#include "Client.hpp"
#include <strings.h>


// STATIC SHARED BUFFER 
char Client::_buffer[BUFFER_SIZE];

Client::Client() {
	
}

Client::Client(int connection_fd) : _server_fd(connection_fd),
									_client_request_timout(time(NULL)),
									_request_status(0),
									_address_len(0),
									_response(NULL),
									_response_cpy(NULL),
									_response_size(0),
									_response_ready(false) {

	bzero(&this->_client_address, sizeof(this->_client_address));
}

Client::~Client() {

	int fd  = this->_responseObj.getFD();
	delete (this->_response_cpy);
	if(fd > 0)
		close(fd);
}

Client::Client(const Client &client): _response(NULL),
									  _response_cpy(NULL) {
	*this = client;
}

Client &Client::operator=(const Client &client) {
	this->_server_fd = client._server_fd;
	this->_connection_fd = client._connection_fd;
	this->_address_len = client._address_len;
	this->_request = client._request;
	this->_client_request_timout = client._client_request_timout;
	this->_server = client._server;
	this->_request_status = client._request_status;
	this->_response_size = client._response_size;
	this->_response = client._response;
	this->_response_cpy = client._response_cpy;
	this->_responseObj = client._responseObj;
	this->_response_ready = client._response_ready;
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
	
	size_t max_body_size = this->_server.getClientMaxBodySize();
	if (this->_request.getRequestState() == HttpRequest::BODY_STATE) {
			if (this->_request.getRequestBodySize() + nb_bytes > max_body_size)
				throw(RequestError(ErrorNumbers::_413_PAYLOAD_TOO_LARGE));
	}
	std::string str_bytes(this->_buffer, nb_bytes);
	this->_request.addRequestData(str_bytes); // CAN'T RELY ON INPLICIT CONVERSION 
}

HttpRequest 	&Client::getRequest() {
	return (this->_request);
}

void	Client::setRequestTimout(time_t  secs) {
	this->_client_request_timout = secs;
}

time_t					Client::getClientRequestTimeout() const {
	return (this->_client_request_timout);
}

char *Client::getResponse() const {
	return (this->_response);
}

size_t Client::getResponseSize() const {
	return (this->_response_size);
}

void  Client::setRequestStatus(short error_num) {
	this->_request_status = error_num;
}


short Client::getRequestStatus() const {
	return (this->_request_status);
}


void Client::setServer(const Server &server) {
	this->_server = server;
}

const Server&	Client::getServer() const {
	return (this->_server);
}

Response &Client::getResponseObj() {
	return (this->_responseObj);
}

void Client::setResponse(char *response, size_t size) {
	this->_response = response;
	this->_response_size = size;
}

void Client::setResponseObj(const Response &resp) {
	this->_responseObj = resp;
	this->_response_ready = true;
}

bool 	Client::ResponseReady() const {
	return (this->_response_ready);
}
