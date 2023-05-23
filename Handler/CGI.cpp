


#include "CGI.hpp"
#include <stdio.h>
#include <unistd.h>



CGI::CGI(std::string path){
	this->script_path = path;
}

void CGI::handlePhpCGI(){
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
		
		// Child process
        close(fd[0]); // Close the read end of the pipe

        // Redirect the output to the write end of the pipe
        dup2(fd[1], STDOUT_FILENO);

        // Execute the PHP script using execle()
        char* args[] = {const_cast<char*>("/opt/homebrew/bin/php-cgi"), const_cast<char*>(this->script_path.c_str()), nullptr};
        execve("/opt/homebrew/bin/php-cgi", args, nullptr);

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

std::string CGI::getResponse(){
	return this->cgi_response;
}
