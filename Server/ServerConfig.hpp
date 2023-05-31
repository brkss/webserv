#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP 
#include "../config/ConfigParse/inc/parse.hpp"

class ServerConfig {


	private:
		const Parse &_config_file;

	public:

		ServerConfig(const Parse &config);
		ServerConfig(const ServerConfig &server_config);
		ServerConfig &operator=(const ServerConfig &server_config);
		~ServerConfig();

		const Server &getServerByname(const std::string &name);
		get


};

ServerConfig::ServerConfig(const Parse &config): config_file(config) {

}

ServerConfig::ServerConfig(const ServerConfig &server_config) {
	*this = server_config;
}

ServerConfig& ServerConfig::operator=(const ServerConfig &server_config) {
	if (this != &server_config) {
		this->_config_file = server_config._config_file;
	}
	return (*this);
}

ServerConfig::~ServerConfig() {

}



#endif /* SERVERCONFIG_HPP */
