


#include "CGI.hpp"
#include <stdio.h>
#include <unistd.h>



CGI::CGI(std::string path){
	this->script_path = path;
}

void CGI::handlePhpCGI(std::string req_body){
	int fd[2];
	if(pipe(fd) == -1){
		perror("piping failed : ");
		return;
	}

	int pid = fork();
	if (pid == -1){
		perror("fork failed : ");
	}
	else if(pid > 0){
		
		std::map<std::string, std::string> headers = generateCGIEnvironement();
		std::map<std::string, std::string>::iterator iter;
		char *env[CGI_ENV_LENGTH + 1];
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


		// Child process
        close(fd[0]); // Close the read end of the pipe

        // Redirect the output to the write end of the pipe
        dup2(fd[1], STDOUT_FILENO);
		dup2(fd[0], STDIN_FILENO);
		
		write(fd[0], req_body.c_str(), req_body.size());


        // Execute the PHP script using execle()
        //char* args[] = {const_cast<char*>("/opt/homebrew/bin/php-cgi"), const_cast<char*>(this->script_path.c_str()), nullptr};
        execve(this->script_path.c_str(), nullptr, env);

		perror("error exec php : ");
        // Execle() will only return if an error occurred
        std::cerr << "Error executing PHP script." << std::endl;

	}else if(pid == 0){
		close(fd[1]);

		char buffer[1024];
		std::string response;
		int bread = read(fd[0], buffer, 1024);
		while(bread > 0){
			bread = read(fd[0], buffer, 1024);
			response += buffer;
		}

		int status;
		waitpid(pid, &status, 0);
		int exit = WEXITSTATUS(status);
		
		this->cgi_response = response;
		std::cout << "cgi response : " << response;
	}
}


std::map<std::string, std::string> CGI::generateCGIEnvironement(){
	
	std::map<std::string, std::string> headers;
	headers["SERVER_PROTOCOL"] = "HTTP/1.1";
	headers["SERVER_PORT"] = "88";
	headers["REQUEST_METHOD"] = "POST";
	headers["PATH_INFO"] = "/";
	headers["PATH_TRANSLATED"] = "/";
	headers["SCRIPT_NAME"] = "test.php";
	headers["QUERY_STRING"] = "";
	headers["REMOTE_HOST"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36";
	headers["REMOTE_ADDR"] = "";
	headers["AUTH_TYPE"] = "";
	headers["REMOTE_USER"] = "";
	headers["REMOTE_IDENT"] = "";
	headers["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
	headers["CONTENT_LENGTH"] = "32";
	headers["REDIRECT_STATUS"] = "200";
	
	return headers;
}



std::string CGI::getResponse(){
	return this->cgi_response;
}
