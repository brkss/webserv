

#include "Handler.hpp"
#include "CGI.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include "utils.h"
#include "../config/ConfigParse/inc/location.hpp"

bool checkAllowedMethods(Location location, std::string method){
    std::vector<std::string> allowedMethods = location.getAllowedMethods();
    if (allowedMethods.size() == 0)
        return true;
    for(size_t i = 0; i < allowedMethods.size(); i++){
        if(method == allowedMethods[i]){
            return true;
        }
    }
    return false;
}

std::string getFilenameFromRequestPath(std::string requestPath){
    size_t slash_pos = requestPath.find_last_of("/");
	if (slash_pos != std::string::npos) {
		std::string filename = requestPath.substr(slash_pos + 1);
        return filename;
    }
    return "";
}

int findMatchingLocation(const std::string& requestPath, const std::vector<Location>& locations) {
    int longestMatchIndex = -1;
    size_t longestMatchLength = 0;

    for (size_t i = 0; i < locations.size(); i++) {
        std::string endpoint = locations[i].getEndpoint();
        size_t endpointLength = endpoint.length();

        if (requestPath.length() >= endpointLength && requestPath.substr(0, endpointLength) == endpoint) {
            if (endpointLength > longestMatchLength) {
                longestMatchIndex = i;
                longestMatchLength = endpointLength;
            }
        }
    }

    return longestMatchIndex;
}

bool fileExists(std::string filepath){
    std::fstream file(filepath);
    return file.good();
}

bool directoryExits(std::string path){
    DIR *directory = ::opendir(path.c_str());
    
    if(directory != NULL){
        closedir(directory);
        return true;
    }
    return false;
}

std::string ListFile(std::string path){
    DIR *directory = ::opendir(path.c_str());
    std::string response = "<!DOCTYPE html><html lang='en'><head><title>index of</title></head><body><h1>Index of</h1><table style='table-layout:fixed;width: 100%;'><tr><td><b>Name</b></td><td><b>Date</b></td><td><b>Size</b></td></tr>";

    if(directory == NULL){
        std::cout << ">>listing files<< : error invalid directory \n";
        return "";
    }
    struct dirent *entry;
    while((entry = readdir(directory)) != NULL){
        std::string filename = entry->d_name;
        std::string fullPath = path + "/" + filename;
        
        struct stat fileInfo;
        if (stat(fullPath.c_str(), &fileInfo) == -1)
        {
            std::cerr << "Error getting file info for: " << fullPath << std::endl;
            continue;
        }

        if(S_ISDIR(fileInfo.st_mode)){
            // directory !
            response += "<tr style='margin-top: 10px;'><td><a href='"+filename+"/'>" + filename + "/</a></td><td>" + std::to_string(fileInfo.st_atime) + "</td><td>--</td></tr>";
        }else {
            // file ! 
            response += "<tr style='margin-top: 10px;'><td><a href='"+filename+"'>" + filename + "</a></td><td>" + std::to_string(fileInfo.st_atime) + "</td><td>" + std::to_string(fileInfo.st_size / 1000000) + " M</td></tr>";
        }
    }
    closedir(directory);
    response += "</table></body></html>";
    return response;
}

bool isDirectory(std::string path){
    size_t slash_pos = path.find_last_of("/");
    if(slash_pos != std::string::npos && slash_pos == path.length() -1){
        return true;
    }
    return false;
}

bool isPHPScript(std::string path){
    size_t dot_pos = path.find_last_of(".");
	if (dot_pos != std::string::npos) {
		std::string extension = path.substr(dot_pos + 1);
        if(extension == "php")
            return true;
    }
    return false;
}


Handler::Handler(Client client){
    this->client = client;
    HttpRequest request = client.getRequest();
    std::string rootPath = this->client.getServer().getRoot();
    std::vector<Location> locations = client.getServer().getLocations();
    std::string upload_location = client.getServer().getUploadStore();



    // check location 
    int locationIndex = findMatchingLocation(request.getRequestPath(), locations);
    if(locationIndex > -1){
        rootPath = locations[locationIndex].getRoot();
        request.setRequestPath(request.getRequestPath().substr(locations[locationIndex].getEndpoint().length() + 1));
        std::cout << "\n\n\n---------------- LOCATION PATH ------------------\n\n\n";
        std::cout << rootPath;
        std::cout << "\n";
        std::cout << "\n\n\n ------------------------------------------------\n\n\n";
        //request.setRequestPath(request.getRequestPath().substr(0,  locations[locationIndex].getEndpoint().length()));
        if(locations[locationIndex].getUploadStore().size() > 0){
            upload_location = locations[locationIndex].getUploadStore();
        }
        // check allowed methods !
        if(!checkAllowedMethods(locations[locationIndex], request.getRequestMethod())){
            this->body = "<html><body style='text-align: center'><h1>405 Method Not Allowed</h1></body></html>";
            this->size = 85;
            this->status = 405;
            this->type = "text/html";
            return;
        }
    }
	// joined location path with resource name 
	std::string path = rootPath + request.getRequestPath();
    std::string method = request.getRequestMethod();
    

    std::cout << "\n\n\n---------------- PATH ------------------\n\n\n";
    std::cout << path;
    std::cout << "\n\n\n ----------------------------------------\n\n\n";


    if((method == "POST" || method == "DELETE") && !isPHPScript(path) && upload_location.length() > 0){
        // handle POST / DELETE
        int status = -1;
        std::string filepath = rootPath + upload_location + getFilenameFromRequestPath(request.getRequestPath());
        
        
        if(filepath.length() == 0){
            this->body = "<html><body style='text-align:center'><h1>404 Not Found</h1><h3>webserv</h3></body></html>";
            this->type = "text/html";
            this->size = 91;
            this->status = 404;
            return;
        }

        if(method == "POST"){
            status = write_file(filepath, request.getRequestBody());
        }else{
            status = delete_file(filepath);
        }

        // check if any of the methods above failed ! 
        if(!status){
            this->status = 500;
            this->body = "<html><body style='text-align:center'><h1>500 Internal Server error</h1><h3>webserv</h3></body></html>";
            this->type = "text/html";
            this->size = 103;
        }else if(status){
            this->status = 201;
            this->body = "<html><body style='text-align:center'><h1>201 Created</h1><h3>webserv</h3></body></html>";
            this->type = "text/html";
            this->size = 89;
        }

        return;
    }
    else if(!fileExists(path) && !directoryExits(path)){
        this->body = "<html><body style='text-align:center'><h1>404 Not Found</h1><h3>webserv</h3></body></html>";
        this->type = "text/html";
        this->size = 91;
        this->status = 404;

        return;
    }else if (isDirectory(path)){
        if(!client.getServer().getAutoIndex()){

            path = rootPath + client.getServer().getIndex();
             
        }else {
            std::string autoIndexResponse = ListFile(path);
            
            this->body = autoIndexResponse;
            this->status = 200;
            this->type = "text/html";
            this->size = autoIndexResponse.size();
            
            return;
        }
    }
    
    if(isPHPScript(path)){
        // handle cgi !
        CGI cgi(client);
        cgi.handlePhpCGI(path);
        std::string response = cgi.getResponse();
        std::map <std::string, std::string> parsed_cgi_response = cgi.parse_cgi_response(response);
       
        this->body = parsed_cgi_response["body"];
        this->type = parsed_cgi_response["type"];
        this->size = parsed_cgi_response["body"].size();
        this->status = 200;
    
    }else{
		
        this->body = this->getFileContent(path);
        //this->fd = this->getFileFD(path);
		this->type = this->getFileContentType(path);
		this->size = this->getFileContentLength(path);
        this->status = 200;
    
    }
}


std::string Handler::getFileContent(std::string filename){
    
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
		std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
	std::cout << "file size " << file_size << std::endl;
    file.seekg(0, std::ios::beg);

    char* buffer = new char[file_size];
    file.read(buffer, file_size);
    file.close();

	std::string file_contents(buffer, file_size);
    delete[] buffer;
    return file_contents;
}

int Handler::getFileContentLength(std::string filename){
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

std::string Handler::getFileContentType(std::string filename){

	std::string file_type = "application/octet-stream"; 
	size_t      dot_pos = filename.find_last_of(".");
	
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
		} else if (extension == "mp4"){
            file_type = "video/mp4";
        } else if (extension == "mp3"){
            file_type = "audio/mpeg";
        } else if (extension == "pdf"){
            file_type = "application/pdf";
        }
	}
	return file_type;
}


int Handler::getSize(){
    return this->size;
}

std::string Handler::getBody(){
    return this->body;
}

std::string Handler::getType(){
    return this->type;
}

int Handler::getStatus(){
    return this->status;
}

int Handler::getFD(){
    return this->fd;
}