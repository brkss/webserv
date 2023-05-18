


#include "CGI.hpp"



int main(){

	
	CGI cgi("./test.php");

	cgi.handlePhpCGI();

	std::cout << "------------------\n\n\n\n";
	std::cout << cgi.getResponse();

	return (1);
}
