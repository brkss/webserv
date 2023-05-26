/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriouic <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 22:45:44 by adriouic          #+#    #+#             */
/*   Updated: 2023/05/26 22:46:06 by adriouic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

#include "ErrorHandler.hpp"
#include "Utils.hpp"

const std::string GET		= "GET";
const std::string POST		= "POST";
const std::string DELETE	= "DELETE";
const size_t URI_MAX_LEN	= 2000; 

HttpRequest::HttpRequest() : _request_state(HEADER_STATE),
							 _transfer_type(UNSET) {
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
	this->_transfer_type = request._transfer_type;
	this->_request_data = request._request_data;
	this->_request_body = request._request_body;
	return (*this);
}

void	validateMethod(const std::string &method) {
	if (method != GET and
		method != POST and
		method != DELETE) {
			throw (RequestError(ErrorNumbers::_400_BAD_REQUEST));
			//throw (ErrorLog(ErrorMessage::ERROR_400));
	}
}

void	validatePath(const std::string &path) {
		if (path.size() > URI_MAX_LEN) throw (RequestError(ErrorNumbers::_414_URI_TOO_LONG));
		//throw(ErrorLog(ErrorMessage::ERROR_414));
		if (path[0] != '/')	throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
		//throw(ErrorLog(ErrorMessage::ERROR_400));
}

void	validateVersion(const std::string &http_version) {
		std::string version_str;
		const std::string HTTP("HTTP/");
		char *rest = NULL;
		double version;

		if (http_version.compare(0, HTTP.size(), HTTP) == 0) {
			version_str = http_version.substr(HTTP.size());
			version = std::strtod(version_str.c_str(), &rest);
			if (*rest) {
				throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
				//throw(ErrorLog(ErrorMessage::ERROR_400));
		}
	 	if (version > 1.1) {
			throw(RequestError(ErrorNumbers::_505_HTTP_VERSION_NOT_SUPPORTED));
			//throw(ErrorLog(ErrorMessage::ERROR_505));
		}
	}
}

void	HttpRequest::parseRequestLine(std::string &request_line) {
		// Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
		std::vector<std::string> values = Utils::split(request_line, " ");
		if (values.size() != 3) {
			throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
			//throw(ErrorLog(ErrorMessage::ERROR_400));
		}
		setRequestMethod(values[0]);
		setRequestPath(values[1]);
		setHttpVersion(values[2]);
}

void HttpRequest::parseHedears(const std::string &headers_str) { 

	typedef std::vector<std::string>::iterator v_iterator;
	std::pair<std::string, std::string> 		temp_pair;
	std::vector<std::string>					headers;
	HttpRequest::headers_t						header_map;
	const std::string 							CRLF("\r\n");
	size_t  									pos;

	headers = Utils::split(headers_str, CRLF);
	for (v_iterator line = headers.begin() ; line != headers.end(); line++)
	{
		if ((pos = line->find(":")) != std::string::npos) {
			temp_pair = std::make_pair(Utils::trimSpaces(line->substr(0, pos)),Utils::trimSpaces(line->substr(pos + 1)));
			header_map.insert(temp_pair);
			//std::cout << "[" << temp_pair.first << "]" << " " << "[" << temp_pair.second << "]" << std::endl;
		}
		else
			throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
			//throw(ErrorLog(ErrorMessage::ERROR_400));
	}
	this->_request_headers = header_map;
	CheckTransferType();
}

void HttpRequest::CheckTransferType() {
	CeckContentLength();
	CheckTransferEncoding();
}

void HttpRequest::CheckTransferEncoding() {
	const std::string transfer_encoding("Transfer-Encoding");
	const std::string chunked_transfer("chunked");
 	const std::string header_value = getHeaderValue(transfer_encoding);

	if (header_value != transfer_encoding) {
		if (header_value ==  chunked_transfer){
			setTransferType(CHUNKED);
		}
		else { // UNSUPORTED ENCODING TYPE 
			//throw(ErrorLog(ErrorMessage::ERROR_400));
			throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
		}
	}
}

void HttpRequest::CeckContentLength() {
	const std::string content_length("Content-Length");

 	const std::string header_value = getHeaderValue(content_length);
	if (header_value != content_length) {
		if (Utils::is_number(header_value)) {
			setTransferType(CONTENT_LENGHT);
		}
		else { 
			//throw(ErrorLog(ErrorMessage::ERROR_400));
			throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
		}
	}
}

void		HttpRequest::parse(std::string &request) {

	const std::string CRLF("\r\n");
	size_t  delim_pos = request.find(CRLF);	
	std::string request_line;
	std::string headers;

	if (delim_pos != std::string::npos) {
		request_line = request.substr(0, delim_pos);	
		headers = request.substr(delim_pos + CRLF.size()); 
		parseRequestLine(request_line);
		parseHedears(headers);
		setRequestState(HttpRequest::BODY_STATE);
	}
	else {
		throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
		//throw(ErrorLog(ErrorMessage::ERROR_400));
	}
}

// GETTERS 
// RETURNS VALUE ASSOSIATED WITH header_key, IF NOT FOUND header_key IS RETURNED 
const std::string &HttpRequest::getHeaderValue(const std::string &header_key) {
	headers_t::const_iterator header = this->_request_headers.find(header_key);
	if (header !=  this->_request_headers.end()) {
		return (header->second);
	}
	return (header_key);
}

std::string			&HttpRequest::getRequestBody() {
			return (this->_request_body);
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

int		HttpRequest::getBodyTransferType() const {
		return (this->_transfer_type);
}
//
// SETTERS
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
void 	HttpRequest::setTransferType(BODY_TRANSFER type) {
		this->_transfer_type =  type;
}

void	HttpRequest::setHttpVersion(const std::string &version) {
	validateVersion(version);
	this->_http_version = version;
}

void 				HttpRequest::appendChunk(const std::string &chunk) {
	this->_request_body = this->_request_body + chunk;
}
