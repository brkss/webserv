#ifndef PARSE_HPP
# define PARSE_HPP 

#include "http.hpp"
#include "server.hpp"
#include "token.hpp"
#include <iostream> 
#include <fstream>
#include <vector>

class  Parse
{
	private:
		size_t				_token_index;
		Http				_common_config;
		std::vector<Server> _servers;	
		std::vector<Token>	_tokens;
		
	public:
		Parse();
		Parse(const Parse &parse);
		Parse &operator=(const Parse &parse);
		~Parse();
		parseFile(const std::string &file_name);

		void addToken(Token &token);
		Token &currToken(void);
		Token &nextToken(void);

		void parseHttpBlock();
		void expectToken(const std::string &expected);

};


#endif /* PARSE_HPP */
