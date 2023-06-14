#ifndef SERVER_HPP
# define SERVER_HPP 

#include "http.hpp"
#include <unistd.h>
#include <vector> 

class Location;


class Server : public Http 
{
	private:
		//listen address:port | port  | address 
		std::string 				_address;
		short						_port;
		std::string 				_server_name;
		std::string					_upload_store;
		bool						_allow_upload;
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
		void 						setReturnCode(short code);
		void 						setReturnURL(const std::string &url);
		void						addLocation(const Location &location);
		bool						isValidServer() const;
		void 						setUploadStore(const std::string &path);
		const std::string&			getUploadStore() const;
		virtual void 				validateUplodeStore(const Server &scop) const;
		bool						allowUpload() const;

};


#endif /* SERVER_HPP */
