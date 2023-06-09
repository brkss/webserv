

#include "Handler.hpp"
#include "CGI.hpp"

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


    //this->path = path;
    //this->req_body = req_body;
    
    // handle 404 file not found response !
    if(isPHPScript(request.getRequestPath())){
        // handle cgi !
        CGI cgi(client);
        cgi.handlePhpCGI();
        std::string response = cgi.getResponse();
        std::map <std::string, std::string> parsed_cgi_response = cgi.parse_cgi_response(response);
       
        this->body = parsed_cgi_response["body"];
        this->type = parsed_cgi_response["type"];
        this->size = parsed_cgi_response["body"].size();
    }else{
        // other media
        this->body = this->getFileContent(request.getRequestPath());
        this->type = this->getFileContentType(request.getRequestPath());
        this->size = this->getFileContentLength(request.getRequestPath());
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