#include "../inc/lexer.hpp"
#include "../inc/token.hpp"
#include "../inc/http.hpp"
#include "../inc/parse.hpp"
#include "../inc/server.hpp"
#include <fstream>
#include <iostream>

int main(int ac, char **av)
{
	Parse parser;
	
	if (ac != 2)
	{
		std::cout << "config file" << std::endl;
		return (1);
	}
	try {
		parser.parseFile(av[1]);
		parser.print_config();
	}
	catch(const std::exception &e){

		std::cout << e.what() << std::endl;
	}
}
