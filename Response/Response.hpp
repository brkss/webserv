#ifndef RESPONSE
#define RESPONSE

#include <string>

class Response {

	std::string contentType;
	std::string contentLength;
	std::string date;
	std::string server;
	std::string connection;
	std::string cacheControl;

	public:
		Response();
		std::string generateResponse(std::string body);

};


#endif // !RESPONSE
