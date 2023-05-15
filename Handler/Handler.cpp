

#include "Handler.hpp"



Handler::Handler(std::string path){
    this->path = path;
    // handle 404 file not found response !
    this->body = this->getFileContent(this->path);
    this->type = this->getFileContentType(this->path);
    this->size = this->getFileContentLength(this->path);
}



std::string Handler::getFileContent(std::string filename){
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

    // Return the file contents and content type
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


int Handler::getSize(){
    return this->size;
}

std::string Handler::getBody(){
    return this->body;
}

std::string Handler::getType(){
    return this->type;
}