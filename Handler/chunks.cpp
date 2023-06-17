


#include <string>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>


int getFileFd(std::string path){
    int fd = -1;

    fd = open(path.c_str(), O_RDONLY);
    return fd;
}

std::string getResponseChunk(int size, int fd){
    char buffer[size];
    std::string response = "";

    int status = read(fd, buffer, (size_t)size);
    if(status > 0){
        response = buffer;
    }else {
        perror("read failed : ");
    }

    return response;
}

int main(){

    std::string path = "./test.txt";
    int fd = getFileFd(path);
    
    std::string response = getResponseChunk(30, fd); 
    while(response.length() > 0){
        std::cout << ">>" << response << "\n";
        response = getResponseChunk(5, fd);
    }

    return 0;
}