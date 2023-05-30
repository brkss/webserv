

#include <string>
#include <map>
#include <iostream>
#include <cstring>

//char ** generateCGIEnvironement(std::map<std::string, std::string> req_headers){

#define CGI_ENV_LENGTH 14

std::map<std::string, std::string> generateCGIEnvironement(){
	
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
	return headers;
}


int main(){

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


	
	i = 0;
	while(env[i]){
		std::cout << ">> " << env[i] << "\n";
		i++;
	}
	
	

	//generateCGIEnvironement();
	return (0);
}
