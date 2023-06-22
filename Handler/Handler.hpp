

#ifndef HANDLER_CPP
#define HANDLER_CPP

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include "../Server/Client.hpp"

class Handler {

    Client client;

    
    std::string body;
    std::string type;
	std::string cookie;

	std::string location;
    int         status;
    int         size;
    int         fd;

    std::string                  rootPath;
    std::map<short, std::string> errorPages;
    int                          return_status;
    std::string                  return_url;


    public:
        Handler();
        Handler(Client &client);
        //Handler(std::string path, std::string req_body, std::map<std::string, std::string> headers);

        std::string getFileContent(std::string filename);
        int getFileContentLength(std::string filename);
        std::string getFileContentType(std::string filename);

        int getSize();
		const std::string &getBody();
        std::string getType();
        int getStatus();
        int getFD();


        int getReturnStatus(){
            return this->return_status;
        }
        std::string getReturnUrl(){
            return this->return_url;
        }

        const std::string &getPath() const {
            return this->rootPath;
        }
        const std::map<short, std::string> &getErrorPages() const {
            return this->errorPages;
        }
		const std::string &getCookie() const  {
			return (this->cookie);
		}
		const std::string &getLocation() const {
			return (this->location);
		}

		~Handler() {

		}
};

#endif
