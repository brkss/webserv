#ifndef LOCATION_HPP
# define LOCATION_HPP 

#include "server.hpp"

class Location: public Server
{
	private:
			
	
	public:
		Location();
		Location &operator=(const Location& location);
		Location(const Location &location);
		~Location();
};

#endif /* LOCATION_HPP */
