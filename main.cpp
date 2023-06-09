#include "config/ConfigParse/inc/lexer.hpp"
#include "config/ConfigParse/inc/token.hpp"
#include "config/ConfigParse/inc/http.hpp"
#include "config/ConfigParse/inc/parse.hpp"
#include "config/ConfigParse/inc/server.hpp"
#include <fstream>
#include <iostream>
#include  "Server/ServerDrive.hpp"

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
		//parser.print_config();
		ServerDrive server(parser);
		server.run();
		
	}
	catch(const std::exception &e){
		std::cout << e.what() << std::endl;
	}
}
