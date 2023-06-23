#ifndef LOCATION_HPP
# define LOCATION_HPP 

#include "server.hpp"
#include <vector>


class Location: public Server
{
	private:
			std::string _endpoint;	
			std::vector<std::string> _allowed_methods;
	
	public:
		Location();
		Location &operator=(const Location& location);
		Location(const Location &location);
		~Location();
		Location(const Server &server_conf);
		void setEndpoint(const std::string &endpoint);
		const std::string &getEndpoint() const ;
		const std::vector<std::string> &getAllowedMethods() const;
		void addMethod(const std::string &method);
};

#endif /* LOCATION_HPP */
