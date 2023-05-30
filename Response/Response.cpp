
#include "Response.hpp"
#include <iomanip>
#include <iostream>
#include <ctime>
#include <fstream>

/*
 *
 * HELPER fucntion that get current date and time in the format required by http !
 *
 */
std::string nowHTTP(){
 	// Get the current time
    std::time_t current_time = std::time(nullptr);

    // Convert to a struct tm in UTC
    std::tm* gm_time = std::gmtime(&current_time);

    // Format the time according to HTTP-date format
    char http_date[30];
    std::strftime(http_date, sizeof(http_date), "%a, %d %b %Y %H:%M:%S GMT", gm_time);

	return http_date;
}


Response::Response(std::string body, std::string type, int size){
	
	this->contentType = type;
	this->contentLength = std::to_string(size);
	// add content length 
	this->date = nowHTTP();
	this->server = "webserv/1.0 (macos)";
	this->connection = "Keep-Alive";
	this->cacheControl = "no-cache, no-store, must-revalidate";
	this->accept = "*/*";
	this->acceptEncoding = "gzip, deflate, sdch";
	this->host = "localhost:89";
	this->body = body;
}

std::string Response::generateResponse(){

	std::string response = "";

	// set header 
	response += "HTTP/1.1 200 OK\n";
	response += "Content-Type: " + this->contentType + "\n";
	response += "Content-Length: " + this->contentLength + "\n";
	response += "Cache-Control: " + this->cacheControl + "\n";
	response += "Date: " +  this->date  + "\n";
	response += "Server: " + this->server + "\n";
	response += "Connection: " + this->connection + "\n";
	response += "Accept: " + this->accept + "\n";
	response += "Accept-Encoding: " + this->acceptEncoding + "\n";
	response += "Host: " + this->host + "\n\n";

	response += this->body + "\n";

	char* c2 = const_cast<char*>(response.c_str());
	std::cout << "response : \n" <<  c2;

	return response;
}

std::string Response::getContentLength(){
	return this->contentLength;
}
