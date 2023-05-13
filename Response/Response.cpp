
#include "Response.hpp"
#include <iomanip>
#include <iostream>
#include <ctime>

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


Response::Response(){
	this->contentType = "text/html";
	// add content length 
	this->date = nowHTTP();
	this->server = "webserv/1.0 (macos)";
	this->connection = "Keep-Alive";
	this->cacheControl = "no-cache, no-store, must-revalidate";
	this->accept = "*/*";
	this->acceptEncoding = "gzip, deflate, sdch";
	this->host = "localhost:89";
}

std::string Response::generateResponse(std::string body){

	std::string response = "";

	// set header 
	response += "HTTP/1.1 200 OK\n";
	response += "Content-Type: " + this->contentType + "\n";
	response += "Content-Length: " + std::to_string(body.length()) + "\n";
	response += "Cache-Control: " + this->cacheControl + "\n";
	response += "Date: " +  this->date  + "\n";
	response += "Server: " + this->server + "\n";
	response += "Connection: " + this->connection + "\n";
	response += "Accept: " + this->accept + "\n";
	response += "Accept-Encoding: " + this->acceptEncoding + "\n";
	response += "Host: " + this->host + "\n\n";

	response += body + "\n";
	
	return response;
}

