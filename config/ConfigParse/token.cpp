#include "token.hpp" 

const std::string Token::ROOT 					= "root";
const std::string Token::LISTEN 				= "listen";
const std::string Token::SERVER_NAME 			= "server_name";
const std::string Token::ERROR_LOG 				= "error_log";
const std::string Token::RETURN 				= "return";
const std::string Token::AUTOINDEX				= "autoindex";
const std::string Token::INDEX 					= "index";
const std::string Token::VALUE 					= "value";
const std::string Token::INT_VALUE				= "INT";
const std::string Token::CLIENT_MAX_BODY_SIZE 	= "client_max_body_size";
const std::string Token::TOKEN_EOF 				= "";
const std::string Token::COLON  				= ";";
const std::string Token::LBRACE 				= "{";
const std::string Token::RBRACE 				= "}";
const std::string Token::UNDEFINED				= "undefined";

const std::string Token::_directives[6][2] = {
	{"root",		Token::ROOT},
	{"listen",		Token::LISTEN},
	{"server_name",	Token::SERVER_NAME},
	{"return",		Token::RETURN},
	{"autoindex",	Token::AUTOINDEX},
	{"index",		Token::INDEX}
};

const  size_t Token::_nb_directives  = 6 ;

const std::string &Token::getTokenType() 
{
	return (this->_type);
}

Token::Token(const std::string &name, const std::string &type)
	: _name(name), _type(type)
{
}

Token::Token()
{
}

Token::Token(const Token& token_)
{
	*this  = token_;
}

Token & Token::operator=(const Token& token_)
{
	if (this != &token_)
	{
		this->_name = token_._name;
		this->_type = token_._type;
	}
	return (*this);
}

Token::~Token() {
}

