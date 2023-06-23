
#include <iostream>
#include <string>

std::string removeEndpoint(const std::string& requestPath, const std::string& endpoint) {
    if (requestPath.substr(0, endpoint.length()) == endpoint) {
        std::string updatedPath = requestPath.substr(endpoint.length());
        return updatedPath;
    }
    return requestPath;
}

int main(){

    std::string request_path = "/cgi-bin/channel.xml";
    std::string endpoint = "/cgi-bin/";


    std::cout << "request path : " << request_path << "\n";
    std::cout << "request path : " << removeEndpoint(request_path, endpoint) << "\n";

    return (0);
}