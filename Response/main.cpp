


#include "Response.hpp"
#include <iostream>
#include <fstream>
int main(){




	Response response("./test.html");

	std::string resp = response.generateResponse();

	std::cout << resp ;
	return (1);
}
