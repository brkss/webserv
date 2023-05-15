

#ifndef HANDLER_CPP
#define HANDLER_CPP

#include <iostream>
#include <string>
#include <fstream>

class Handler {


    std::string path;

    std::string body;
    std::string type;
    int size;


    public:
        Handler(std::string path);
        std::string getFileContent(std::string filename);
        int getFileContentLength(std::string filename);
        std::string getFileContentType(std::string filename);

        int getSize();
        std::string getBody();
        std::string getType();

};


#endif