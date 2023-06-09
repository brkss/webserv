



#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <unistd.h>
#include <map>



void handleCGI(std::string body, std::string script, char **env){
    
    FILE *fileIN = ::tmpfile();
    FILE *fileOUT = ::tmpfile();
    int fdIN = fileno(fileIN);
    int fdOUT = fileno(fileOUT);
    
    std::string response;


    write(fdIN, body.c_str(), body.size());
    lseek(fdIN, 0, SEEK_SET);

    pid_t pid = fork();
    if(pid == -1){
        std::cout << "something went wrong forking !\n";
        exit(0);
    }else if(pid == 0){
        if(dup2(fdIN, STDIN_FILENO) == -1){
            perror("dup2 stdout failed : ");
            exit(0);
        }
        if(dup2(fdOUT, STDOUT_FILENO) == -1){
            perror("dup2 stdout failed : ");
            exit(0);
        }
        fclose(fileIN);
        fclose(fileOUT);
        close(fdIN);
        close(fdOUT);
        
        execve(script.c_str(), nullptr, env);
        perror("execve err : ");
        std::cerr << "something went wront executing the cgi script !";
        exit(0);
    }else if (pid > 0){
        // main proccess!
       
        int status;
		waitpid(pid, &status, 0);
		WEXITSTATUS(status);

        char buffer[1024];
        lseek(fdOUT, 0, SEEK_SET);
		int bread = read(fdOUT, buffer, 1024);
        if(bread == -1){
            perror("read failed : ");
            exit(0);
        } 
		while(bread > 0){
            response += buffer;
			bread = read(fdOUT, buffer, 1024);
		}
		fclose(fileIN);
        fclose(fileOUT);
        close(fdIN);
        close(fdOUT);

		
		
		std::cout << "response : \n" << response << "\n\n\n";
    }
    
}


char **generateCGIEnvironement(){

	std::map<std::string, std::string> headers;
	headers["SERVER_PROTOCOL"] = "HTTP/1.1";
	headers["SERVER_PORT"] = "88";
	headers["REQUEST_METHOD"] = "POST";
	headers["PATH_INFO"] = "/";
	headers["PATH_TRANSLATED"] = "/Users/brkss/Developer/1337/webserv-merge/lab/test.php";
	headers["SCRIPT_NAME"] = "/test.php";
	headers["QUERY_STRING"] = "";
	headers["REMOTE_HOST"] = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36";
	headers["REMOTE_ADDR"] = "127.0.0.1";
	headers["AUTH_TYPE"] = "";
	headers["REMOTE_USER"] = "127.0.0.1";
	headers["REMOTE_IDENT"] = "127.0.0.1";
	headers["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
	headers["CONTENT_LENGTH"] = "11";
	headers["REDIRECT_STATUS"] = "200";
	
	std::map<std::string, std::string>::iterator iter;
	char **env = new char*[15 + 1];
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

int main(){

    std::string script = "/Users/brkss/Developer/1337/webserv-merge/lab/test.php";
    std::string body = "email=allo";
    char **env = generateCGIEnvironement();

    int i = 0;
    while(env[i]){
        //std::cout << ">> " << env[i] << "\n";
        i++;
    }

    handleCGI(body, script, env);

}