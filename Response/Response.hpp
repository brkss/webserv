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
	std::string accept;
	std::string acceptEncoding;
	std::string host;

	std::string body;



	public:
		Response(std::string body);
		std::string generateResponse();
		std::string getContentLength();

};


#endif // !RESPONSE
