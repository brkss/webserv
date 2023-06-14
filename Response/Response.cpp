
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

Response::Response(std::string body, std::string type, int size, int status){
	
	this->status = std::to_string(status);
	this->status_message = generateStatusMessage(status);
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
	response += "HTTP/1.1 " + this->status +  " " + this->status_message + "\r\n";
	response += "Content-Type: " + this->contentType + "\r\n";
	response += "Content-Length: " + this->contentLength + "\r\n";
	response += "Cache-Control: " + this->cacheControl + "\r\n";
	response += "Date: " +  this->date  + "\r\n";
	response += "Server: " + this->server + "\n";
	response += "Connection: " + this->connection + "\r\n";
	response += "Accept: " + this->accept + "\r\n";
	response += "Accept-Encoding: " + this->acceptEncoding + "\r\n";
	response += "Host: " + this->host + "\r\n\r\n";

	response += this->body + "\r\n";

	return response;
}

std::string Response::getContentLength(){
	return this->contentLength;
}


std::string Response::generateStatusMessage(int status){
	if(status == 404)
		return "Not Found";
	else if (status == 500)
		return "Internal Server Error";
	else if (status == 403)
		return "Forbidden";
	else if (status == 401)
		return "Unauthorized";
	else if (status == 503)
		return "Service Unavailable"; 
	else if (status == 405)
		return "Method Not Allowed";
	switch  (status) { 
		case (400): return   "Bad request";
		case (408): return   "Request timeout";
		case (411): return   "Length required";
		case (414): return   "Uri_too long";
		case (413): return   "Payload too large";
		case (431): return   "Request header fields too large";
		case (500): return   "Internal server error";
		case (501): return   "No timplemented";
		case (505): return   "Http version not supported";
		default:
			return "OK";
	}


}
