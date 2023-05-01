#include "parse.hpp"

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
		this->_servers = parse.servers;
		this->_tokens = parse._tokens;
	}
}

Parse::~Parse() {

}

void Parse::addToken(Token &token)
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

void Parse::expectToken(const std::string &expected)
{
	std::string error;
	if (currToken.getTokenType() != expected)
	{
		error = "Unexpected Token '" + expected + "' " ;
		throw(error);
	}
}

void Parse::parseServer()
{
	

}

std::string Parse::parseRoot() {
	std::string value;

	nextToken();
	expectToken(Token::VALUE);
	value = currToken().getTokenValue(); 
	nextToken();
	expectToken(Token::COLON);
	return (value);
}

bool Parse::parseAutoIndex() {
	std::string value;
	bool 		ret;

	nextToken();
	expectToken(Token::VALUE);
	value = currToken().getTokenValue();

	if (value == "on")
		ret = true;
	else if ( value == "off")
		ret = false;
	else
		throw("NOT a valid value");
	expectToken(Token::COLON);
}

void Parse::parseHttpBlock() {

	expectToken(Token::HTTP);
	nextToken();
	expectToken(Token::LBRACE);

	curr_token = currToken();
	while (curr_token.getTokenType() != Token::TOKEN_EOF and curr_token.getTokenType() != Token::RBRACE)
	{
		if (curr_token.getTokenType() == Token::SERVER)
			parseServer();
		else if (curr_token.getTokenType() == Token::ROOT)
			parseRoot();
		else if (curr_token.getTokenType() == Token::AUTOINDEX)
			parseAutoIndex();
		else if (curr_token.getTokenType() == Token::ERROR_LOG)
			parseErrorLog();
		else if (curr_token.getTokenType() == Token::INDEX)
			parseIndex();
		else if (curr_token.getTokenType() == Token::CLIENT_MAX_BODY_SIZE)
			parseClientBodySize();
		else
			throw("Undifined Token");
		nextToken();
	}
	expectToken(Token::RBRACE);
}

void Parse::parseFile(const std::string &file_name){

	std::ifstream	file;
	std::string		error;
	std::string		name;
	Lexer 			lexer;
	Token			token;

	file.open(av[1]);
	if (!file.is_open())
	{
		error = "Unable to open file : " + file_name; 
		throw(error);
	}
	while ( not std::getline(file, line).eof() )
	{
		lexer.setLine(line);	
		token = lexer.getNextToken();
		while (token.getTokenType() != Token::TOKEN_EOF)
		{
			this->_tokens.addToken(token);
			token = lexer.getNextToken();
		}
	}
	this->_tokens.addToken(Token::TOKEN_EOF);
	parseHttpBlock();
}
