#include <string>
#include <arpa/inet.h>	// for ip4 wrapper function 
#include <algorithm>	// for find method
#include <unistd.h>
#include "../inc/parse.hpp"
#include "../inc/location.hpp"
#include "../inc/lexer.hpp"
#include "../../Server/Utils.hpp"
#include "../../Server/ErrorHandler.hpp"

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

void Parse::parseUploadStore(Server &server) {
	nextToken();
	expectToken(Token::VALUE);
	server.setUploadStore(currToken().getTokenValue());
	nextToken();
	expectToken(Token::COLON);
}

void Parse::parsePort(Server &conf) {
	nextToken();
	expectToken(Token::VALUE);
	std::string value = currToken().getTokenValue();
	if (Utils::is_number(value))
		conf.setPort(std::atoi(value.c_str()));
	else
		throw_error(currToken(), "Bad argument");
	nextToken();
	expectToken(Token::COLON);
}

void Parse::parseListen(Server &conf) {
	nextToken();
	expectToken(Token::VALUE);
	std::string value = currToken().getTokenValue();
	if (is_valid_ip_address(value))
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
		else if (curr_token.getTokenType() == Token::UPLOAD_STORE)
			parseUploadStore(location_conf);
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
		else if (curr_token.getTokenType() == Token::PORT)
			parsePort(server_conf);
		else if (curr_token.getTokenType() == Token::RETURN)
			parseReturn(server_conf);
		else if (curr_token.getTokenType() == Token::UPLOAD_STORE)
			parseUploadStore(server_conf);
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

void   Parse::parseErrorPage(Http &config) {

	std::string  error_code;
	std::string	file_path;

	nextToken();
	expectToken(Token::VALUE);
	error_code = currToken().getTokenValue();
	if (!Utils::is_number(error_code)) {
		throw_error(currToken(), "Bad argument! http_error_code expected ");
	}

	nextToken();
	expectToken(Token::VALUE);
	file_path = currToken().getTokenValue();
	if (0 && access(file_path.c_str(), F_OK | W_OK)) {
		throw_error(currToken(), "Unable to access File!");
	}
	nextToken();
	expectToken(Token::COLON);
	config.addErrorPaage(std::atoi(error_code.c_str()), file_path);
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

void Parse::parseClientRequestTimout(Http &config) {

	std::string value;

	nextToken();
	expectToken(Token::VALUE);
	value  = currToken().getTokenValue();
	if (Utils::is_number(value))
		config.setRequestTimout(std::atol(value.c_str()));
	else
		throw_error(currToken(), "Bad argument");
	nextToken();
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
	else if (curr_token.getTokenType() == Token::ERROR_PAGE)
		parseErrorPage(config);
	else if (curr_token.getTokenType() == Token::INDEX)
		parseIndex(config);
	else if (curr_token.getTokenType() == Token::CLIENT_MAX_BODY_SIZE)
		parseClientBodySize(config);
	else if (curr_token.getTokenType() == Token::CLIENT_REQUEST_TIMOUT)
		parseClientRequestTimout(config);
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
	ValidateConfigRequirements();
}

void Parse::ValidateConfigRequirements() {
	if (!this->_servers.size()) throw(ErrorLog("Missing Server config! QUITING ..."));
	for (cv_iterator server = this->_servers.begin(); server != this->_servers.end(); server++)
		 if (!server->isValidServer()) 
				throw(ErrorLog("Ivalid Config File. QUITTING ..."));
}

 std::vector<Server> &Parse::getVirtualServers()  {
	return (this->_servers);
}
