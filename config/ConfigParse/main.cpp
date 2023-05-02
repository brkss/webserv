#include "lexer.hpp"
#include "token.hpp"
#include "http.hpp"
#include "parse.hpp"
#include "server.hpp"
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
	}

	catch(const std::exception &e){

		std::cout << e.what() << std::endl;
	}
}
