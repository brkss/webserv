#include "ConfigParse/inc/lexer.hpp"
#include "ConfigParse/inc/token.hpp"
#include "ConfigParse/inc/http.hpp"
#include "ConfigParse/inc/parse.hpp"
#include "ConfigParse/inc/server.hpp"
#include <fstream>
#include <iostream>
#include  "Server/ServerDrive.hpp"

int main(int ac, char **av)
{
	Parse parser;
	
	if (ac != 2) {
		std::cout << "No config File" << std::endl;
		return (1);
	}
	try {
		parser.parseFile(av[1]);
		ServerDrive server(parser);
		server.run();
	}
	catch(const std::exception &e){
		std::cout << e.what() << std::endl;
	}
}
