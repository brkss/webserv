#include "parse.hpp"
#include "lexer.hpp"
#include <string>

void throw_error(const Token &token, std::string error) throw(std::invalid_argument)
{
	error = error + " '" +  token.getTokenValue() + "'";
	error +=  " at  line " + std::to_string(token.getLineNum()) ;
	throw(std::invalid_argument(error));

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

void Parse::parseServer()
{	

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
		config.setAutoindex(true);
	else if ( value == "off")
		config.setAutoindex(false);
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
	std::string::const_iterator it;

	nextToken();
	expectToken(Token::VALUE);
	value  = currToken().getTokenValue();
	it = value.begin();
	while (std::isdigit(*it))
		it++;
	if (it == value.end())
		config.setClientMaxBodySize(std::atol(value.c_str()));
	else
		throw_error(currToken(), "Bad argument");
	nextToken();
	expectToken(Token::COLON);
}

void Parse::parseGlobalDirective() {

	Http &global_config = this->_common_config;
	Token curr_token;

	curr_token = currToken();
	if (curr_token.getTokenType() == Token::ROOT)
		parseRoot(global_config);
	else if (curr_token.getTokenType() == Token::AUTOINDEX)
		parseAutoIndex(global_config);
	else if (curr_token.getTokenType() == Token::ERROR_LOG)
		parseErrorLog(global_config);
	else if (curr_token.getTokenType() == Token::INDEX)
		parseIndex(global_config);
	else if (curr_token.getTokenType() == Token::CLIENT_MAX_BODY_SIZE)
		parseClientBodySize(global_config);
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
			parseGlobalDirective();
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
