#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP 

#include <map>

class HttpRequest {
	
	private:
		bool	   							_request_ready;
		std::string							_request_method;
		std::string							_request_path;
		std::string							_http_version;
		std::map<std::string, std::string>	_request_headers; 
		
	
	public:
		typedef std::map<std::string, std::string> headers_t; // MAP OF HEADERS

		HttpRequest();
		~HttpRequest();
		HttpRequest(const HttpRequest &request);
		HttpRequest &operator=(const HttpRequest &request);

		const std::string 	&getRequestMethod() const ;
		const std::string 	&getRequestPath() const ;
		const std::string 	&getHttpVersion() const ;
		const headers_t		&getHeaders() const ; 

		bool 				isReady() const;

};

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

const headers_t		&HttpRequest::getHeaders() const { 

	return (this->_request_headers);
}

bool HttpRequest::isReady() const {
	return (this->_request_ready);
}

#endif /* HTTPREQUEST_HPP */
