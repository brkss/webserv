#include <iostream>
#include <vector>
#include <string>

class Location {
public:
    Location(const std::string& endpoint) : endpoint(endpoint) {}

    std::string getEndPoint() const {
        return endpoint;
    }

private:
    std::string endpoint;
};

int findMatchingLocation(const std::string& requestPath, const std::vector<Location>& locations) {
    int longestMatchIndex = -1;
    int longestMatchLength = 0;

    for (int i = 0; i < locations.size(); i++) {
        std::string endpoint = locations[i].getEndPoint();
        int endpointLength = endpoint.length();

        if (requestPath.length() >= endpointLength && requestPath.substr(0, endpointLength) == endpoint) {
            if (endpointLength > longestMatchLength) {
                longestMatchIndex = i;
                longestMatchLength = endpointLength;
            }
        }
    }

    return longestMatchIndex;
}

int main() {
    // Example usage
    std::string requestPath = "/something/bla/yo";
    Location loc1("/something");
    Location loc2("/something/blabla");
    Location loc3("/another");
    std::vector<Location> locations;
    locations.push_back(loc1);
    locations.push_back(loc2);
    locations.push_back(loc3);
    

    int matchingIndex = findMatchingLocation(requestPath, locations);

    if (matchingIndex != -1) {
        std::cout << "Match found at index: " << matchingIndex << std::endl;
        std::cout << "Endpoint: " << locations[matchingIndex].getEndPoint() << std::endl;
    } else {
        std::cout << "No match found." << std::endl;
    }

    return 0;
}
