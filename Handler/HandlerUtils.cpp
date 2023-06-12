
#include "utils.h"

int write_file(std::string path, std::string content){

	std::ofstream outputFile(path);

    if (outputFile.is_open()) {
        outputFile << content;
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