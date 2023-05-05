#include "http.hpp"
#include "defaultConfig.hpp"

Http::Http() : 	_erro_log(DefaulConfig::ERROR_LOG),
				_root(DefaulConfig::ROOT),
				_index(DefaulConfig::INDEX),
				_autoindex(DefaulConfig::AUTOINDEX),
				_client_max_body_size(DefaulConfig::CLIENT_MAX_BODY_SIZE)
{

}
Http::Http(const Http &http) {

	*this = http;
}
Http &Http::operator=(const Http &http) {
	if (this != &http)
	{
		this->_erro_log = http._erro_log;
		this->_root = http._root;
		this->_index = http._index;
		this->_autoindex = http._autoindex;
		this->_client_max_body_size = http._client_max_body_size;
		this->_cgi = http._cgi;
	}
	return (*this);
}
Http::~Http() {

}

const std::string &Http::getErroLog() const {
	return (this->_erro_log);
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

void Http::setErroLog(const std::string &value) {
	this->_erro_log = value;
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

