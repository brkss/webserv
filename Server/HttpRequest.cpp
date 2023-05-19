#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {

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
	this->_request_ready =  request._request_ready;

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

bool HttpRequest::isReady() const {
	return (this->_request_ready);
}

void HttpRequest::addRequestData(const std::string &data) {
	this->_request_data = this->_request_data + data; 
	//std::cout << "Size: "<< this->_request_data.size()<< "\nData: " << this->_request_data << std::endl;
	//this->_request_data.clear();
}
