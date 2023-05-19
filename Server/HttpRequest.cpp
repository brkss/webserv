#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : _request_state(HEADER_STATE) {

}

HttpRequest::~HttpRequest() {


}
HttpRequest::HttpRequest(const HttpRequest &request) {

	*this = request;
}
	
HttpRequest &HttpRequest::operator=(const HttpRequest &request) {

	this->_request_method =  request._request_method;
	this->_request_path =  request._request_path;
	this->_http_version = request._http_version;
	this->_request_headers =  request._request_headers;
	this->_request_state =  request._request_state;

	return (*this);
}

const std::string 	&HttpRequest::getRequestMethod() const { 
	return (this->_request_method);
}
const std::string 	&HttpRequest::getRequestPath() const { 

	return (this->_request_path);
}
const std::string 	&HttpRequest::getHttpVersion() const { 

	return (this->_http_version);
}

const HttpRequest::headers_t		&HttpRequest::getHeaders() const { 

	return (this->_request_headers);
}

void HttpRequest::addRequestData(const std::string &data) {
	this->_request_data = this->_request_data + data; 
}

std::string			&HttpRequest::getRequestData() {
	return (this->_request_data);
}

int	HttpRequest::getRequestState() const {
	return(this->_request_state);
}

void				HttpRequest::setRequestState(REQUEST_STATE state) {
	this->_request_state = state;
}
