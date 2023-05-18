#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <iostream>
#include <unistd.h>

class CGI {

	
	std::string cgi_response;
	std::string script_path;

	public:
		CGI(std::string path);
		void handlePhpCGI();
		std::string getResponse();


};


#endif
