#include "http.hpp"

Http::Http(){

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

const bool		  &Http::getAutoindex() const {
	return (this->_autoindex);
}

const size_t	  &Http::getClientMaxBodySize() const {
	return (this->_client_max_body_size);
}


