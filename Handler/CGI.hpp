#ifndef CGI_HPP
#define CGI_HPP

#define CGI_ENV_LENGTH 14

#include <string>
#include <iostream>
#include <map>
#include <unistd.h>

class CGI {

	
	std::string cgi_response;
	std::string script_path;

	public:
		CGI(std::string path);
		void handlePhpCGI(std::string req_body);
		std::string getResponse();

		std::map<std::string, std::string> generateCGIEnvironement();
};


#endif
