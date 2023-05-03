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
		void parseFile(const std::string &file_name);

		void addToken(Token token);
		Token &currToken(void);
		Token &nextToken(void);

		void parseHttpBlock();
		void parseGlobalDirective(Http &config);
		void expectToken(const std::string &expected) throw(std::invalid_argument);

		void parseServer();
		void parseRoot(Http &config) ;
		void parseAutoIndex(Http &config);
		void parseErrorLog(Http &config);
		void parseIndex(Http &config);
		void parseClientBodySize(Http &config);

		void parseListen(Server &conf);
		void parseServerName(Server &conf);
		void parseReturn(Server &conf);
		void parseLocation();

		void print_config();

		void addServer(const Server &server);


};


#endif /* PARSE_HPP */
