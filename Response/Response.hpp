#ifndef RESPONSE
#define RESPONSE

#include <string>
#include <unistd.h>
#include <map>

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
	int status;
	std::string status_message;
	std::string cookie;
	std::string redirect;

	std::string rootPath;
	std::map<short, std::string> errorPages;

	int fd;
	bool	headears_sent;
	static std::ifstream static_file;

	static std::string response_chunk;
	static std::string body;

	public:
		//Response(std::string body, std::string type, int size, int status, int fd);

		Response(std::string body, std::string type,const std::string &cookie, const std::string &redirect, int size, int status, int fd, std::string path, std::map<short, std::string> errorPages);
		~Response()  {  } 
		std::string generateResponse();
		
		const std::string &getResponseBody();
		std::string getResponseHeaders();
		
		std::string getContentLength();
		std::string generateStatusMessage(int status);
		const std::string &getResponseChunk(int size);
		
		void checkErrorPage();

		int getFD();
		Response(const Response &response);
		Response & operator=(const Response &response);
		Response() {};
		std::string getStatusCode();


};


#endif // !RESPONSE
