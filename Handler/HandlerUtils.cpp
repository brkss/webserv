
#include "utils.h"

int write_file(std::string path, std::string filename){

    std::ifstream inputFile(filename);
	std::ofstream outputFile(path);
    if (outputFile.is_open() && inputFile.is_open()){
        std::string line;
        while (std::getline(inputFile, line)) {
            outputFile << line << std::endl;
        }
        inputFile.close();
        outputFile.close();
    } else 
		return 0;

	return 1;
}

int delete_file(std::string path){

	int status = ::remove(path.c_str());
	
	if (status != 0) 
    	return 0;

	return 1;
}

bool isCGIScript(std::string path){
    
    
    size_t dot_pos = path.find_last_of(".");
	if (dot_pos != std::string::npos) {
		std::string extension = path.substr(dot_pos + 1);
        if(extension == "php" || extension == "py")
            return true;
    }
    return false;
}


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

int getFileFd(std::string path){
    int fd = -1;
    fd = open(path.c_str(), O_RDONLY);
    return fd;
}


std::string generateStatusMessage(int status){
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

std::string generateErrorResponse(int status){    

    std::string body = "<html><body><h1 style='text-align: center;'>";

    body += std::to_string(status) + " ";
    body += generateStatusMessage(status);
    
    body += "<html><body><h1>";
    return body;
}