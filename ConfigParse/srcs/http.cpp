#include "../inc/http.hpp"
#include "../inc/defaultConfig.hpp"

Http::Http() : 	_root(DefaulConfig::ROOT),
				_index(DefaulConfig::INDEX),
				_autoindex(DefaulConfig::AUTOINDEX),
				_client_max_body_size(DefaulConfig::CLIENT_MAX_BODY_SIZE),
				_client_request_timout (DefaulConfig::CLIENT_REQUEST_TIMOUT)
{

}

Http::Http(const Http &http) {

	*this = http;
}
Http &Http::operator=(const Http &http) {
	if (this != &http)
	{
		this->_error_pages = http._error_pages;
		this->_root = http._root;
		this->_index = http._index;
		this->_autoindex = http._autoindex;
		this->_client_max_body_size = http._client_max_body_size;
		this->_cgi = http._cgi;
		this->_client_request_timout = http._client_request_timout;
	}
	return (*this);
}
Http::~Http() {

}

const std::map<short, std::string>  &Http::getErroPages() const {

	return (this->_error_pages);
}

const std::string &Http::getRoot() const {
	return (this->_root);
}
const std::string &Http::getIndex() const {
	return (this->_index);
}

const bool		  &Http::getAutoIndex() const {
	return (this->_autoindex);
}

const size_t	  &Http::getClientMaxBodySize() const {
	return (this->_client_max_body_size);
}

void Http::addErrorPaage(short error_code, const std::string &path) {

	//page_pair_t error_path(error_code, path);
	this->_error_pages[error_code] = path;
}
void Http::setRoot(const std::string &value) {
	this->_root = value;
}
void Http::setIndex(const std::string &value) {
	this->_index = value;
}
void Http::setAutoIndex(bool value)  {
	this->_autoindex = value;
}

void Http::setClientMaxBodySize(size_t value) {
	this->_client_max_body_size = value;
}
void Http::addCgi(const pair &cgi) {
	this->_cgi.insert(cgi);
}
void Http::setRequestTimout(size_t timeout) {
	this->_client_request_timout = timeout;
}

