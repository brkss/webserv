#include "../inc/token.hpp" 

const std::string Token::HTTP 					= "http";
const std::string Token::SERVER 				= "server";
const std::string Token::LOCATION				= "location";
const std::string Token::ROOT 					= "root";
const std::string Token::LISTEN 				= "listen";
const std::string Token::SERVER_NAME 			= "server_name";
const std::string Token::ERROR_LOG 				= "error_log";
const std::string Token::RETURN 				= "return";
const std::string Token::AUTOINDEX				= "autoindex";
const std::string Token::INDEX 					= "index";
const std::string Token::CLIENT_MAX_BODY_SIZE 	= "client_max_body_size";
const std::string Token::LIMIT_EXCEPT 			= "limit_except";
const std::string Token::CGI	 				= "cgi";
const std::string Token::TOKEN_EOF 				= "EOF";
const std::string Token::COLON  				= ";";
const std::string Token::LBRACE 				= "{";
const std::string Token::RBRACE 				= "}";
const std::string Token::UNDEFINED				= "undefined";
const std::string Token::INT_VALUE				= "INT";
const std::string Token::VALUE 					= "value";

const std::string Token::_directives[12][2] = {

	{"http",		Token::HTTP},
	{"server",		Token::SERVER},
	{"location",	Token::LOCATION},
	{"root",		Token::ROOT},
	{"listen",		Token::LISTEN},
	{"server_name",	Token::SERVER_NAME},
	{"return",		Token::RETURN},
	{"autoindex",	Token::AUTOINDEX},
	{"index",		Token::INDEX},
	{"error_log",		Token::ERROR_LOG},
	{"limit_except",		Token::LIMIT_EXCEPT},
	{"client_max_body_size", Token::CLIENT_MAX_BODY_SIZE}
};

const  size_t Token::_nb_directives  = 12;

const std::string &Token::getTokenType() const {
	return (this->_type);
}

const std::string &Token::getTokenValue() const {
	return (this->_value);
}

const size_t &Token::getLineNum() const {
	return (this->_line_num);
}

bool Token::isGlobalDirective() const {
	std::string type = this->_type;

	if (type ==  Token::CGI or type  == Token::ROOT or type == Token::ERROR_LOG or 
		type ==  Token::AUTOINDEX or  type == Token::INDEX or type == Token::CLIENT_MAX_BODY_SIZE )
		return (true);
	return (false);
}

void Token::setLineNum(size_t  line_num) {
	this->_line_num = line_num;
}

Token::Token(const std::string &type, const std::string &value)
	: _value(value), _type(type), _line_num(0) {
}

Token::Token() :_line_num(0){ 
}

Token::Token(const Token& token_) {
	*this  = token_;
}

Token & Token::operator=(const Token& token_) {
	if (this != &token_) {
		this->_value = token_._value;
		this->_type = token_._type;
		this->_line_num = token_._line_num;
	}
	return (*this);
}

Token::~Token() {
}

