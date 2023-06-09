#ifndef CGI_HPP
#define CGI_HPP

#define CGI_ENV_LENGTH 15

#include <string>
#include <iostream>
#include <map>
#include <unistd.h>
#include "../Server/Client.hpp"

class CGI {

	Client client;

	std::string cgi_response;

	public:
		CGI(Client client);
		void handlePhpCGI();
		std::string getResponse();
		std::map<std::string, std::string> parse_cgi_response(std::string response);

		char **generateCGIEnvironement();
};


#endif
