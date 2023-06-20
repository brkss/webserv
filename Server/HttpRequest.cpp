#include "HttpRequest.hpp"

#include "ErrorHandler.hpp"
#include "Utils.hpp"

#include <stdlib.h>
const std::string 	HttpRequest::GET			= "GET";
const std::string 	HttpRequest::POST			= "POST";
const std::string 	HttpRequest::DELETE			= "DELETE";
const size_t		HttpRequest::URI_MAX_LEN	= 2000; 

HttpRequest::HttpRequest() : _request_state(HEADER_STATE),
							 _transfer_type(UNSET),
							 _data_file_fd(-1) {
				
}

HttpRequest::~HttpRequest() {
	close(this->_data_file_fd);

}

HttpRequest::HttpRequest(const HttpRequest &request) {
	*this = request;
}
	
HttpRequest &HttpRequest::operator=(const HttpRequest &request) {

	this->_request_method =	request._request_method;
	this->_request_path =	request._request_path;
	this->_http_version =	request._http_version;
	this->_request_headers =  request._request_headers;
	this->_request_state =  request._request_state;
	this->_transfer_type = request._transfer_type;
	this->_request_data = request._request_data;
	this->_request_body = request._request_body;
	this->_query_string =  request._query_string;
	this->_data_file_fd = request._data_file_fd;
	this->_data_filename = request._data_filename;
	this->_request_line = request._request_line;
	return (*this);
}

void	validateMethod(const std::string &method) {
	if (method != HttpRequest::GET and
		method != HttpRequest::POST and
		method != HttpRequest::DELETE) {
			throw (RequestError(ErrorNumbers::_400_BAD_REQUEST));
	}
}

void	validatePath(const std::string &path) {
		if (path.size() > HttpRequest::URI_MAX_LEN)	throw (RequestError(ErrorNumbers::_414_URI_TOO_LONG));
		if (path[0] != '/')				throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
}

void	validateVersion(const std::string &http_version) {
		std::string version_str;
		const std::string HTTP("HTTP/");
		char *rest = NULL;
		double version;

		if (http_version.compare(0, HTTP.size(), HTTP) == 0) {
			version_str = http_version.substr(HTTP.size());
			version = std::strtod(version_str.c_str(), &rest);
			if (*rest) 
				throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
		 	if (version > 1.1) 
				throw(RequestError(ErrorNumbers::_505_HTTP_VERSION_NOT_SUPPORTED));
	}
}

void	HttpRequest::parseRequestLine(std::string &request_line) {
		// Request-Line   = Method SP Request-URI SP HTTP-Version CRLF
		std::vector<std::string> values = Utils::split(request_line, " ");
		if (values.size() != 3) {
			throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
		}
		setRequestMethod(values[0]);
		setRequestPath(values[1]);
		setQueryString(values[1]);
		setHttpVersion(values[2]);
}

void HttpRequest::parseHeaders(const std::string &headers_str) { 

	typedef std::vector<std::string>::iterator	v_iterator;
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
		}
		else
			throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
	}
	this->_request_headers = header_map;
	// Header validation may be required !! depends on response 

	validateHeaders();		// checks for presense of required headres 
	CheckTransferType();
}

void HttpRequest::validateHeaders() {
	const std::string host("Host");
 	const std::string header_value = getHeaderValue(host);

	if ((header_value == host) or header_value.empty()) {
			throw(RequestError(ErrorNumbers::_400_BAD_REQUEST)); // Host header required!
	}	
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
		if (header_value ==  chunked_transfer) 
			setTransferType(CHUNKED);
		else									// UNSUPORTED ENCODING TYPE 
			throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
	}
}

void HttpRequest::CeckContentLength() {
	const std::string content_length("Content-Length");

 	const std::string header_value = getHeaderValue(content_length);
	if (header_value != content_length) {
		if (Utils::is_number(header_value))
			setTransferType(CONTENT_LENGHT);
		else 
			throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));
	}
}

int HttpRequest::getDataFileDescriptor() {
	return (this->_data_file_fd);
}

void		HttpRequest::parse(std::string &request_header) {

	const	std::string CRLF("\r\n");
	size_t				delim_pos;
	std::string			request_line;
	std::string 		headers;
	
	delim_pos = request_header.find(CRLF);	
	if  (delim_pos == std::string::npos) 
		throw(RequestError(ErrorNumbers::_400_BAD_REQUEST));

	#if DEBUG
	std::cerr << request_header << std::endl;
	#endif
	request_line = request_header.substr(0, delim_pos);	
	headers = request_header.substr(delim_pos + CRLF.size()); 
	this->_request_line = request_line; // debug;
	parseRequestLine(request_line);
	parseHeaders(headers);
	setRequestState(HttpRequest::BODY_STATE);
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
	this->_request_path =  path;
	size_t pos = this->_request_path.find('?');
	if (pos != std::string::npos) {
		this->_request_path = path.substr(0, pos);
	//this->_request_path = "/tmp" + path;
	}
}

const std::string &HttpRequest::getQueryString() const  {
	return (this->_query_string);
}

void HttpRequest::setQueryString(const std::string &url) {
   std::size_t pos = url.find('?');
   this->_query_string  = ""; 
   if (pos != std::string::npos) {
	   this->_query_string  = url.substr(pos + 1);
  }
}

void 	HttpRequest::setTransferType(BODY_TRANSFER type) {
		this->_transfer_type =  type;
}

void	HttpRequest::setHttpVersion(const std::string &version) {
	validateVersion(version);
	this->_http_version = version;
}

void 				HttpRequest::appendChunk(const std::string &chunk) {
	// writting  data to disk
	//write(
	this->_request_body = this->_request_body + chunk;
}

void HttpRequest::writeChunkTofile(const std::string &data) {

	size_t nb_bytes = write(this->_data_file_fd, data.c_str(), data.size());
	if (nb_bytes != data.size()) {
		#if DEBUG 
		ConsoleLog::Debug("Failed to write data to MemFile");
		#endif 
		throw(RequestError(ErrorNumbers::_500_INTERNAL_SERVER_ERROR));
	}
}

void 				HttpRequest::openDataFile() {

		if (this->_data_file_fd != -1)
			return ;		// file already open

		this->_data_filename = Utils::randomFileName();
		this->_data_file_fd = open(this->_data_filename.c_str(), O_RDWR | O_CREAT, 0600); // file opened for read & write
		if (this->_data_file_fd == -1) {
			perror(NULL);
			std::cout << "file name " << this->_data_filename  << std::endl;
			ConsoleLog::Debug("Failed to open MemFile");
			throw(RequestError(ErrorNumbers::_500_INTERNAL_SERVER_ERROR));
		}
		#if DEBUG 
		std::cout << "MemFile file name : " << this->_data_filename << std::endl;
		#endif 
}

const std::string & HttpRequest::getRequestLine() const {
	return (this->_request_line);
}
