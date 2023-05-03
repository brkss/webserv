#ifndef SERVER_HPP
# define SERVER_HPP 

#include "http.hpp"
#include <vector> 

class Location;


class Server : public Http
{
	private:
		//listen address:port | port  | address 
		std::string 				_address;
		short						_port;
		std::string 				_server_name;
		std::vector<Location>		_locations;

	protected:
		//return code URL;
		std::string 				_return_URL;
		short						_return_code;
		
	public:
		Server();
		Server(const Server& server);
		Server(const Http& http);
		Server &operator=(const Server & server);
		virtual ~Server();

		const std::string			&getAddress() const;
		const short 				&getPort() const;
		const std::string 			&getServerName() const;
		const std::vector<Location> &getLocations() const;
		const std::string			&getReturnURL() const ;
		const short					&getReturnCode() const ;
		void						setAddress(const std::string &address);
		void 						setPort(short server_port);
		void 						setServerName(const std::string &server_name);
		void						addLocation(Location &location);

};


#endif /* SERVER_HPP */
