
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

/*
 *
 *	HELPER function that read file content !
 *
*/

int get_file_content_length(std::string filename){

	std::ifstream file(filename, std::ios::binary);
    if (!file) {
		std::cerr << "Failed to open file: " << filename << std::endl;
		return 0;
    }

    // Determine the file size
    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    file.close();

	return file_size;
}

std::string get_file_content(std::string filename){

	std::ifstream file(filename, std::ios::binary);
    if (!file) {
		std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    // Determine the file size
    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the entire file into memory
    char* buffer = new char[file_size];
    file.read(buffer, file_size);
    file.close();

    // Determine the file type based on the file extension
	std::string file_type = "application/octet-stream"; // Default content type
    size_t dot_pos = filename.find_last_of(".");
    if (dot_pos != std::string::npos) {
		std::string extension = filename.substr(dot_pos + 1);
        if (extension == "html") {
            file_type = "text/html";
        } else if (extension == "css") {
            file_type = "text/css";
        } else if (extension == "js") {
            file_type = "text/javascript";
        } else if (extension == "png") {
            file_type = "image/png";
        } else if (extension == "jpg" || extension == "jpeg") {
            file_type = "image/jpeg";
        }
    }

    // Return the file contents and content type
	std::string file_contents(buffer, file_size);
    delete[] buffer;
    return file_contents;
}

/*
 *
 *	Helper function get file content type !
 *
 * 
 */

std::string get_file_type(std::string filename){
	// Determine the file type based on the file extension
	std::string file_type = "application/octet-stream"; // Default content type
	size_t dot_pos = filename.find_last_of(".");
	if (dot_pos != std::string::npos) {
		std::string extension = filename.substr(dot_pos + 1);
		if (extension == "html") {
			file_type = "text/html";
		} else if (extension == "css") {
			file_type = "text/css";
		} else if (extension == "js") {
			file_type = "text/javascript";
		} else if (extension == "png") {
			file_type = "image/png";
		} else if (extension == "jpg" || extension == "jpeg") {
			file_type = "image/jpeg";
		}
	}
	return file_type;
}

Response::Response(std::string body){

	std::string file_content = get_file_content(body);
	std::string content_type = get_file_type(body);
	int size = get_file_content_length(body);
	std::cout << "file size : " << std::to_string(get_file_content_length(body)) ;

	this->contentType = content_type;
	this->contentLength = std::to_string(get_file_content_length(body));
	// add content length 
	this->date = nowHTTP();
	this->server = "webserv/1.0 (macos)";
	this->connection = "Keep-Alive";
	this->cacheControl = "no-cache, no-store, must-revalidate";
	this->accept = "*/*";
	this->acceptEncoding = "gzip, deflate, sdch";
	this->host = "localhost:89";
	this->body = file_content;
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
