

#ifndef HANDLER_CPP
#define HANDLER_CPP

#include <iostream>
#include <string>
#include <fstream>
#include <map>

class Handler {

    std::string req_body;
    std::map<std::string, std::string> header;

    char **env;

    std::string path;
    std::string body;
    std::string type;
    int size;
    int err;


    public:
        Handler(std::string path);
        Handler(std::string path, std::string req_body, std::map<std::string, std::string> headers);

        std::string getFileContent(std::string filename);
        int getFileContentLength(std::string filename);
        std::string getFileContentType(std::string filename);

        int getSize();
        std::string getBody();
        std::string getType();
        

};


#endif