#include "../inc/parse.hpp"
#include "../inc/location.hpp"
#include "../inc/lexer.hpp"
#include "../../../Server/Utils.hpp"
#include <string>
#include <arpa/inet.h> // for ip4 wrapper function 
#include <algorithm> // for find method


void Parse::print_config() {
	typedef  std::vector<Server>::const_iterator cv_iterator;
	typedef  std::vector<Location>::const_iterator cl_iterator;
	std::cerr << "--- Common config ---"    << std::endl;
	std::cout << "Error log : " << this->_common_config.getErroLog() << std::endl;
	std::cout << "Index		: " << this->_common_config.getIndex()  << std::endl;
	std::cout << "Auto index: " << this->_common_config.getAutoIndex()  << std::endl;
	std::cout << "Root		: " << this->_common_config.getRoot() << std::endl;
	std::cout << "ClientBody : " << this->_common_config.getClientMaxBodySize() << std::endl;
	std::cerr << "--- Servers config ---"    << std::endl;
	
	for (cv_iterator it  = this->_servers.begin() ; it != this->_servers.end(); it++)
	{
		std::cout << ">>> server <<< " << std::endl;

		std::cout << "-----:server:" << "Error log : " << (*it).getErroLog() << std::endl;
		std::cout << "-----:server:" << "Index		: " << (*it).getIndex()  << std::endl;
		std::cout << "-----:server:" << "Auto index: " << (*it).getAutoIndex()  << std::endl;
		std::cout << "-----:server:" << "Root		: " << (*it).getRoot() << std::endl;
		std::cout << "-----:server:" << "ClientBoy : " << (*it).getClientMaxBodySize() << std::endl;
		std::cout << "-----:server:" << "ServerName: " << (*it).getServerName() << std::endl;
		std::cout << "-----:server:" << "Adderss: " << (*it).getAddress() << std::endl;
		std::cout << "-----:server:" << "Port: " << (*it).getPort() << std::endl;
		std::cout << "-----:server:" << "ReturnCode: " << (*it).getReturnCode() << std::endl;
		std::cout << "-----:server:" << "ReturnURL: " << (*it).getReturnURL() << std::endl;
		for (cl_iterator lit = it->getLocations().begin(); lit != it->getLocations().end(); lit++)
		{
			std::cout << "			>>> Location <<< " << std::endl;
			std::cout << "-----------:Location:" << "Location endpoint : " << (*lit).getEndpoint() << std::endl;
			std::cout << "-----------:Location:" << "Error log : " << (*lit).getErroLog() << std::endl;
			std::cout << "-----------:Location:" << "Index		: " << (*lit).getIndex()  << std::endl;
			std::cout << "-----------:Location:" << "Auto index: " << (*lit).getAutoIndex()  << std::endl;
			std::cout << "-----------:Location:" << "Root		: " << (*lit).getRoot() << std::endl;
			std::cout << "-----------:Location:" << "ClientBoy : " << (*lit).getClientMaxBodySize() << std::endl;
			std::cout << "-----------:Location:" << "ServerName: " << (*lit).getServerName() << std::endl;
			std::cout << "-----------:Location:" << "Adderss: " << (*lit).getAddress() << std::endl;
			std::cout << "-----------:Location:" << "Port: " << (*lit).getPort() << std::endl;
			std::cout << "-----------:Location:" << "ReturnCode: " << (*lit).getReturnCode() << std::endl;
			std::cout << "-----------:Location:" << "ReturnURL: " << (*lit).getReturnURL() << std::endl;
			std::cout << "-----------:Allowed Methods: " ;
			for (std::vector<std::string>::const_iterator m = lit->getAllowedMethods().begin();
				m != lit->getAllowedMethods().end(); m++) {
				std::cout << *m << " " ;
				}
			std::cout << std::endl;

		}
	}


}

void throw_error(const Token &token, std::string error) throw(std::invalid_argument)
{
	error = error + " '" +  token.getTokenValue() + "'";
	error +=  " at  line " + std::to_string(token.getLineNum()) ;
	throw(std::invalid_argument(error));

}

bool is_valid_ip_address(const std::string& ip_address) {
    struct in_addr address;
    return inet_pton(AF_INET, ip_address.c_str(), &address) != 0;
}


Parse::Parse(): _token_index(0) {

}

Parse::Parse(const Parse &parse) {
	*this = parse;
}

Parse &Parse::operator=(const Parse &parse) {
	if (this != &parse)
	{
		this->_token_index = parse._token_index;
		this->_common_config = parse._common_config;
		this->_servers = parse._servers;
		this->_tokens = parse._tokens;
	}
	return (*this);
}

Parse::~Parse() {

}

void Parse::addToken(Token token)
{
	this->_tokens.push_back(token);
}

Token &Parse::currToken(void)
{
	return (this->_tokens[this->_token_index]);
}

Token &Parse::nextToken(void)
{
	this->_token_index++;
	return (currToken());
}

void Parse::expectToken(const std::string &expected) throw(std::invalid_argument)
{
	std::string error;
	Token token = currToken();
	if (token.getTokenType() != expected)
	{
		error = "Unexpected token '" +  token.getTokenValue() + "'";
		error +=  " at  line " + std::to_string(token.getLineNum()) ;
		error += ",Expected '" + expected  + "'." ;
		throw(std::invalid_argument(error));
	}

}

void	Parse::parseServerName(Server &conf)
{
	nextToken();
	expectToken(Token::VALUE);
	conf.setServerName(currToken().getTokenValue());
	nextToken();
	expectToken(Token::COLON);
}

void Parse::parseListen(Server &conf) {
	std::string value;

	nextToken();
	expectToken(Token::VALUE);
	value = currToken().getTokenValue();
	if (Utils::is_number(value))
		conf.setPort(std::atoi(value.c_str()));
	else if (is_valid_ip_address(value))
		conf.setAddress(value);
	else
		throw_error(currToken(), "Bad argument");
	nextToken();
	expectToken(Token::COLON);
}


void Parse::parseReturn(Server &conf){
	Token curr_token;

	nextToken();
	expectToken(Token::VALUE);
	curr_token = currToken();
	if (Utils::is_number(curr_token.getTokenValue()))
	{
		nextToken();
		expectToken(Token::VALUE);
		conf.setReturnCode(std::atoi(curr_token.getTokenValue().c_str()));
		conf.setReturnURL(currToken().getTokenValue());
	}
	else 
		conf.setReturnURL(curr_token.getTokenValue());
	nextToken();
	expectToken(Token::COLON);

}

bool isValidMethod(const std::string &str) {
	const char* valid_methods[] = {"GET", "HEAD", "POST", "PUT", "DELETE"};
	std::vector<std::string> methods(valid_methods, valid_methods + sizeof(valid_methods) /  sizeof(char*)) ;
	return (std::find(methods.begin(), methods.end(), str) != methods.end());
}

void Parse::parseAcceptedMethods(Location &location) {
	Token curr_token ;

	nextToken();
	curr_token = currToken();
	while (curr_token.getTokenType() == Token::VALUE)
	{
		if (isValidMethod(curr_token.getTokenValue()))
			location.addMethod(curr_token.getTokenValue());
		else 
			throw_error(currToken(), "Bad argument");
		nextToken();
		curr_token = currToken();
	}
	expectToken(Token::COLON);
}

void Parse::parseLocation(Server &server_conf) {

	Location location_conf(server_conf);
	Token curr_token;
	std::string endpoint;

	nextToken();
	expectToken(Token::VALUE);	
	location_conf.setEndpoint(currToken().getTokenValue());
	nextToken();
	expectToken(Token::LBRACE);	
	nextToken();
	curr_token = currToken();
	while (curr_token.getTokenType() != Token::TOKEN_EOF && curr_token.getTokenType() != Token::RBRACE) {

		if (curr_token.isGlobalDirective())
			parseGlobalDirective(location_conf);
		else if (curr_token.getTokenType() == Token::RETURN)
			parseReturn(location_conf);
		else if (curr_token.getTokenType() == Token::LIMIT_EXCEPT) 
			parseAcceptedMethods(location_conf);
		else 
		{

			std::cerr << curr_token.getTokenValue() << std::endl;	
			std::cerr << curr_token.getTokenType() << std::endl;	
			throw_error(curr_token, "Unexpected Token");
		}
		nextToken();
		curr_token = currToken();
	}
	expectToken(Token::RBRACE);
	server_conf.addLocation(location_conf);
}

void Parse::addServer(const Server &server) {
	this->_servers.push_back(server);
}
void Parse::parseServer() {

	Server server_conf(this->_common_config);
	Token curr_token;

	nextToken();
	expectToken(Token::LBRACE);	
	nextToken();
	curr_token = currToken();
	while (curr_token.getTokenType() != Token::TOKEN_EOF && curr_token.getTokenType() != Token::RBRACE)
	{
		if (curr_token.getTokenType() == Token::LOCATION)
			parseLocation(server_conf);
		else if (curr_token.isGlobalDirective())
			parseGlobalDirective(server_conf);
		else if (curr_token.getTokenType() == Token::SERVER_NAME)
			parseServerName(server_conf);
		else if (curr_token.getTokenType() == Token::LISTEN)
			parseListen(server_conf);
		else if (curr_token.getTokenType() == Token::RETURN)
			parseReturn(server_conf);
		else 
			throw_error(curr_token, "Unexpected Token");
		nextToken();
		curr_token = currToken();
	}
	expectToken(Token::RBRACE);
	addServer(server_conf);
}

void Parse::parseRoot(Http &config) {
	nextToken();
	expectToken(Token::VALUE);
	config.setRoot(currToken().getTokenValue());
	nextToken();
	expectToken(Token::COLON);
}

void Parse::parseAutoIndex(Http &config) {
	std::string value;

	nextToken();
	expectToken(Token::VALUE);
	value = currToken().getTokenValue();

	if (value == "on")
		config.setAutoIndex(true);
	else if ( value == "off")
		config.setAutoIndex(false);
	else
		throw_error(currToken(), "Ivalid argument");
	nextToken();
	expectToken(Token::COLON);
}

void   Parse::parseErrorLog(Http &config) {

	nextToken();
	expectToken(Token::VALUE);
	config.setErroLog(currToken().getTokenValue());
	nextToken();
	expectToken(Token::COLON);
}
void   Parse::parseIndex(Http &config) {

	nextToken();
	expectToken(Token::VALUE);
	config.setIndex(currToken().getTokenValue());
	// Add Support for "index file ... ;"  if needed 
	nextToken();
	expectToken(Token::COLON);
}

	
void   Parse::parseClientBodySize(Http &config) {

	std::string value;

	nextToken();
	expectToken(Token::VALUE);
	value  = currToken().getTokenValue();
	if (Utils::is_number(value))
		config.setClientMaxBodySize(std::atol(value.c_str()));
	else
		throw_error(currToken(), "Bad argument");
	nextToken();
	expectToken(Token::COLON);
}

void Parse::parseCgi(Http &config) {
	std::string extention;
	std::string path;

	nextToken();
	expectToken(Token::VALUE);
	extention = currToken().getTokenValue();
	nextToken();
	expectToken(Token::VALUE);
	path = currToken().getTokenValue();
	config.addCgi(std::make_pair(extention, path));
	expectToken(Token::COLON);
}

void Parse::parseGlobalDirective(Http &config) {

	Token curr_token;

	curr_token = currToken();
	if (curr_token.getTokenType() == Token::ROOT)
		parseRoot(config);
	else if (curr_token.getTokenType() == Token::AUTOINDEX)
		parseAutoIndex(config);
	else if (curr_token.getTokenType() == Token::ERROR_LOG)
		parseErrorLog(config);
	else if (curr_token.getTokenType() == Token::INDEX)
		parseIndex(config);
	else if (curr_token.getTokenType() == Token::CLIENT_MAX_BODY_SIZE)
		parseClientBodySize(config);
	else if (curr_token.getTokenType() == Token::CGI)
		parseCgi(config);
}

void Parse::parseHttpBlock() {

	Token curr_token;
	std::string  error;

	expectToken(Token::HTTP);
	nextToken();
	expectToken(Token::LBRACE);
	nextToken();

	curr_token = currToken();
	while (curr_token.getTokenType() != Token::TOKEN_EOF and curr_token.getTokenType() != Token::RBRACE)
	{
		if (curr_token.getTokenType() == Token::SERVER)
			parseServer();
		else if (curr_token.isGlobalDirective())
			parseGlobalDirective(this->_common_config);
		else 
			throw_error(curr_token, "Unexpected Token");
		nextToken();
		curr_token = currToken();
	}
	expectToken(Token::RBRACE);
}

void Parse::parseFile(const std::string &file_name){

	Lexer 			lexer;
	Token			token;
	std::ifstream	file;
	std::string		line;
	std::string		error;
	size_t			line_num;
	
	file.open(file_name);
	if (!file.is_open())
	{
		error = "Unable to open file : " + file_name; 
		throw(std::invalid_argument(error));
	}
	line_num = 1;
	while ( not std::getline(file, line).eof() )
	{
		lexer.setLine(line, line_num);	
		token = lexer.getNextToken();
		while (token.getTokenType() != Token::TOKEN_EOF)
		{
			addToken(token);
			token = lexer.getNextToken();
		}
		line_num++;
	}
	token = Token(Token::TOKEN_EOF, Token::TOKEN_EOF);
	token.setLineNum(line_num);
	addToken(token);
	parseHttpBlock();
}

const std::vector<Server> &Parse::getServers() const {
	return (this->_servers);
}
