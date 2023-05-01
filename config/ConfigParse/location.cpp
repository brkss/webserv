#include "location.hpp"

Location::Location() : Server() {

}

Location::Location(const Location &location) : Server(location) {
	*this = location;
}

Location &Location::operator=(const Location& location) 
{
	if (this != &location)
	{
		Server::operator=(location);
	}
	return (*this);

}

Location::~Location() {

}

