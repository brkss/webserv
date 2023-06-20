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
		typedef std::vector<Server>::const_iterator cv_iterator;
		typedef std::vector<Server>::iterator  v_iterator;
		
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
		void parseErrorPage(Http &config);
		void parseIndex(Http &config);
		void parseClientBodySize(Http &config);
		void parseClientRequestTimout(Http &config);

		void parseListen(Server &conf);
		void parsePort(Server &conf);
		void parseServerName(Server &conf);
		void parseReturn(Server &conf);
		void parseLocation(Server &server_conf);
		void parseCgi(Http &Conf);
		void parseAcceptedMethods(Location &location);
		void parseUploadStore(Server &server);

		void ValidateConfigRequirements();

		void print_config();

		void addServer(const Server &server);
		
		std::vector<Server> &getVirtualServers();
};


#endif /* PARSE_HPP */
