



#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <unistd.h>
#include <map>


std::string cgi_response = "X-Powered-By: PHP/8.2.5\n"
                           "Content-type: text/html;charset=UTF-8\n"
                           "\n"
                           "<html><body>\n"
                           "<h1>Hello, World! berkassebrahim@gmail.com</h1>\n"
                           "</body></html>";


std::map<std::string, std::string> parse_cgi_response(std::string response){
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

    
    std::size_t bodyIndex = response.find("\n\n");
    if (bodyIndex != std::string::npos) {
        bodyIndex += 2;
        body = response.substr(bodyIndex);
    }

    // Print the extracted values
    results["type"] = contentType;
    results["body"] = body;
    return results;
}

int main(){
    std::map<std::string, std::string> res = parse_cgi_response(cgi_response);
    std::cout << "Content type : " << res["type"] << "\n";
    std::cout << "Body : " << res["body"] << "\n";

}