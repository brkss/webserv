


#include "Response.hpp"
#include <iostream>
#include <fstream>
int main(){


	Response response("./test-3.jpg");

	std::string resp = response.generateResponse();

	std::cout << "content length : " << response.getContentLength();

	std::cout << resp ;
	return (1);
}
