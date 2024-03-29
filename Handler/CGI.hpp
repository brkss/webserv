#ifndef CGI_HPP
#define CGI_HPP

#define CGI_ENV_LENGTH 20

#include <string>
#include <iostream>
#include <map>
#include <unistd.h>
#include "../Server/Client.hpp"

class CGI {

	Client &client;

	std::string cgi_response;
	int status;

	public:
		
		CGI(Client &client);
		
		void handlePhpCGI(std::string path);
		std::map<std::string, std::string> parse_cgi_response(std::string response);
		char **generateCGIEnvironement(std::string path);

		std::string getResponse();
		int getStatus();
	
		
};


#endif
