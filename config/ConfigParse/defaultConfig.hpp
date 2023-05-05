#ifndef DEFAULTCONFIG_HPP
# define DEFAULTCONFIG_HPP 

#include <iostream>

class DefaulConfig
{
	public:
	   	static const std::string ERROR_LOG;
	   	static const std::string ROOT;
		static const std::string INDEX;
		static const bool		 AUTOINDEX;
	   	static const size_t 	 CLIENT_MAX_BODY_SIZE;
		static const std::string ADDRESS; 
		static const short		 PORT;
	   	static const std::string SERVER_NAME;
};

#endif /* DEFAULTCONFIG_HPP */
