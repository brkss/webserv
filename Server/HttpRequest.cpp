#include "HttpRequest.hpp"

#include "ErrorHandler.hpp"
#include "Utils.hpp"

const std::string GET = "GET";
const std::string POST = "POST";
const std::string DELETE = "DELETE";

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

void	validateMethod(const std::string &method) {
		if (method != GET &&
			method != POST &&
			method != DELETE) {
			throw (ErrorLog("400 Bad Request"));
		}
}

void	validatePath(const std::string &path) {
		// max len should be validated			
		if (path[0] != '/') throw("400 bad request");
}

void	validateVersion(const std::string &version) {
		// invalid 
		// not sipported 
		// or requre downgrade 
		(void) version;
}

void	HttpRequest::parseRequestLine(std::string &request_line) {
		// Request-Line   = Method SP Request-URI SP HTTP-Version CRLF

		std::cout << "request line : " << request_line << std::endl;
		std::vector<std::string> values = Utils::split(request_line, " ");
		if (values.size() != 3) {
			throw(ErrorLog("400 Bad Request"));
		}
		setRequestMethod(values[0]);
		setRequestPath(values[1]);
		setHttpVersion(values[2]);
}

void HttpRequest::parseHedears(const std::string &headers_str) { 

	typedef std::vector<std::string>::iterator v_iterator;

	const std::string CRLF("\r\n");
	HttpRequest::headers_t header_map;
	std::vector<std::string> headers;
	std::pair<std::string, std::string> temp_pair;
	size_t  pos;

	headers = Utils::split(headers_str, CRLF);
	for (v_iterator line = headers.begin() ; line != headers.end(); line++)
	{
		if ((pos = line->find(":")) != std::string::npos) {
			temp_pair = std::make_pair(line->substr(0, pos) ,line->substr(pos + 1));
		}
		else
			throw(ErrorLog("400 Bad Request"));
		header_map.insert(temp_pair);
	}
	this->_request_headers = header_map;
}

void		HttpRequest::parse(std::string &request) {

	const std::string CRLF("\r\n");
	size_t  delim_pos = request.find(CRLF);	
	std::string request_line;
	std::string headers;

	// HAS REQUEST LINE AND HEADERS
	if (delim_pos != std::string::npos) {
		request_line  = request.substr(0, delim_pos);	
		headers = request.substr(delim_pos + CRLF.size()); 
		parseRequestLine(request_line);
		parseHedears(headers);
	}
	else {
		throw(ErrorLog("400 Bad Request"));
	}
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

void	HttpRequest::setRequestMethod(const std::string &method) {

	validateMethod(method);
	this->_request_method = method;
}
void	HttpRequest::setRequestPath(const std::string &path) {

	validatePath(path);
	this->_request_path = path;
}

void	HttpRequest::setHttpVersion(const std::string &version) {
	validateVersion(version);
	this->_http_version = version;
}

