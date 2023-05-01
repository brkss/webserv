#include "lexer.hpp"
#include "token.hpp"
#include "http.hpp"
#include "server.hpp"
#include <fstream>
#include <iostream>

int main(int ac, char **av)
{
	std::fstream  	file;
	std::string		line;
	Lexer			lexer;
	Token			token;

	Http http;
	Http http1;

	Http test(http1);
	(void) test;
	(void) http;

	if (ac != 2)
	{
		std::cout << "config file" << std::endl;
		return (1);
	}
	
	file.open(av[1]);
	if (!file.is_open())
	{
		std::cout << "cant open file" << std::endl;
		return (1);
	}
	
 	while ( not std::getline(file, line).eof() )
	{
		lexer.setLine(line);	
		token = lexer.getNextToken();
		while (token.getTokenType() != Token::TOKEN_EOF)
		{
			std::cout << "--: TkenType : " <<  token.getTokenType() << std::endl;
			std::cout << "--: TkenName : " <<  token.getTokenName() << std::endl;
			std::cout << std::endl;

			token = lexer.getNextToken();
		}
	}

}
