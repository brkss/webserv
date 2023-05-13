


#include "Response.hpp"
#include <iostream>




int main(){


	Response response;

	std::string resp = response.generateResponse("<h1>testing response</h1>");

	std::cout << resp ;


	return (1);
}
