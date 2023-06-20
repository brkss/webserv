


#include "CGI.hpp"
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <sstream>


CGI::CGI(Client &client) : client(client){
	this->client = client;
}

void CGI::handlePhpCGI(std::string path){
    
	HttpRequest &request = this->client.getRequest();
	char **env = generateCGIEnvironement(path);
    FILE *fileOUT = ::tmpfile();
    

    int fdIN =  open(request.getRequestDataFilename().c_str(), O_RDONLY);
    std::cout << "file descriptor file descriptor request : " << fdIN << "\n";
    int fdOUT = fileno(fileOUT);
   
    
    std::string response;
    std::cout << " path : " << path << "\n";

    pid_t pid = fork();
    if(pid == -1){
        std::cout << "something went wrong forking !\n";
        //exit(0);
    }else if(pid == 0){
		// child proc
        if(dup2(fdIN, STDIN_FILENO) == -1 && fdIN != -1){
            // handle 500
            perror("something went wrong dup input : ");
            exit(0);
        }
        if(dup2(fdOUT, STDOUT_FILENO) == -1){
            // handle 500
            exit(0);
        }
        
        fclose(fileOUT);
        close(fdIN);
        close(fdOUT);
        
        execve(path.c_str(), nullptr, env);
        
        //err = 500;
        std::string errorContent = "Content-Type: text/html\r\n\r\n<html><body style='text-align:center;'><h1>500 Internal Error</h1></body></html>";
        write(STDOUT_FILENO, errorContent.c_str(), errorContent.size());
        perror("execve err : ");
        std::cerr << "something went wront executing the cgi script !";
        exit(0);
    }else if (pid > 0){
		// main proccess!
        int status;
		waitpid(pid, &status, 0);
		WEXITSTATUS(status);
       
        char buffer[2];
        lseek(fdOUT, 0, SEEK_SET);
		int bread = read(fdOUT, buffer, 1);
        
        if(bread == -1){
            perror("read failed : ");
            exit(0);
        } 
        while(bread > 0){
            response += buffer;
            bread = read(fdOUT, buffer, 1);
        }
        
        fclose(fileOUT);
        close(fdIN);
        close(fdOUT);

        std::cout << ">>>>>response : " << response << "\n";
		
		this->cgi_response = response; 
		std::cout << "cgi response >>> : " << response; 
    }
    
}


char **CGI::generateCGIEnvironement(std::string path){
	
	Server server = this->client.getServer();
	HttpRequest request = this->client.getRequest();
	std::map<std::string, std::string> req_headers = request.getHeaders();

	std::map<std::string, std::string> headers;
	headers["SERVER_PROTOCOL"] = "HTTP/1.1";
	headers["SERVER_PORT"] = std::to_string(server.getPort());
	headers["REQUEST_METHOD"] = request.getRequestMethod();
	headers["PATH_INFO"] = path;
	headers["PATH_TRANSLATED"] = path;
	headers["SCRIPT_NAME"] = path;
	headers["QUERY_STRING"] = request.getQueryString();
	headers["REMOTE_HOST"] = req_headers["User-Agent"];
	headers["REMOTE_ADDR"] = "";
	headers["AUTH_TYPE"] = "";
	headers["REMOTE_USER"] = "";
	headers["REMOTE_IDENT"] = "";
	headers["CONTENT_TYPE"] = req_headers["Content-Type"];
	headers["CONTENT_LENGTH"] = std::to_string(request.getRequestBody().size());
	headers["REDIRECT_STATUS"] = "200";

    headers["HTTP_ACCEPT_LANGUAGE"] = req_headers["Accept-Language"];
    headers["HTTP_ACCEPT_CHARSET"] = req_headers["Accept-Charset"];
    headers["CONTENT_LANGUAGE"] = req_headers["Content-Language"];

	std::map<std::string, std::string>::iterator iter;
	char **env = new char*[CGI_ENV_LENGTH + 1];
	int i = 0;
	for(iter= headers.begin(); iter != headers.end(); ++iter){
		std::string key = iter->first;
		std::string val = iter->second;

		std::string keyValue = key + "=" + val;
		env[i] = new char[keyValue.size() + 1];
		std::strcpy(const_cast<char*>(env[i]), keyValue.c_str());
		
		i++;	
	}
	env[i] = NULL;
	return env;
}



std::map<std::string, std::string> CGI::parse_cgi_response(std::string response){
    

	std::map <std::string, std::string> results;
    std::string contentType;
    std::string body;

    std::size_t contentTypeIndex = response.find("Content-type: ");
    if (contentTypeIndex != std::string::npos) {
        contentTypeIndex += 14;
        std::size_t newlineIndex = response.find('\n', contentTypeIndex);
        if (newlineIndex != std::string::npos) {
            contentType = response.substr(contentTypeIndex, newlineIndex - contentTypeIndex);
        }
    }
    
    std::size_t bodyIndex = response.find("\r\n\r\n");
    if (bodyIndex != std::string::npos) {
        bodyIndex += 2; 
        
        body = response.substr(bodyIndex);
    }else {
		std::cout << "body no pos !!\n";
	}

   
    results["type"] = contentType;
    results["body"] = body;
    return results;
}

std::string CGI::getResponse(){
	return this->cgi_response;
}

int CGI::getStatus(){
    return this->status;
}
