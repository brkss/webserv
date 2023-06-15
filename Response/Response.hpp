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
	std::string status;
	std::string status_message;

	int fd;

	std::string body;

	public:
		Response(std::string body, std::string type, int size, int status);
		std::string generateResponse();
		std::string getContentLength();
		std::string generateStatusMessage(int status);
		int getFD();

};


#endif // !RESPONSE
