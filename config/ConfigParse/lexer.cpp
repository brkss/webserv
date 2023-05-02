#include "lexer.hpp"



#include <iostream> 

Lexer::Lexer(): _line_size(0),  _index(0), _c(0)  {

}

Lexer::Lexer(const Lexer& lexer_){
	*this = lexer_;
}

Lexer& Lexer::operator=(const Lexer& lexer_) {
	this->_line = lexer_._line;
	this->_line_size = lexer_._line_size;
	this->_index = lexer_._index;
	this->_c = lexer_._c;
	return (*this);
}

Lexer::~Lexer() {

}

void Lexer::readChar() {
	if (this->_index < this->_line_size)
		this->_c = this->_line[_index++];
	else
	{
		this->_index++;
		this->_c = 0; // END of line
	}
}

void Lexer::readWord()	{
	while (std::isalpha(this->_c))
		readChar();
}

char Lexer::getChar() {
	return (this->_c);
}

void	Lexer::setLine(const std::string &line, size_t line_num)	{
	this->_line = line;
	this->_line_num = line_num;
	this->_line_size = line.size();
	this->_index = 0;
	this->_c = this->_line[this->_index];
}

std::string Lexer::getTokenName() {

	size_t origin_index = this->_index - 1;
	size_t len = 0;

	while (this->_c and (std::isalnum(this->_c) || std::strchr("-_:/.", this->_c))) {
		readChar();	
		len++;
	}
	// Reimplement this later
	this->_index--;
	return (this->_line.substr(origin_index, len));
}

bool Lexer::isDirective(const std::string &name) const {

	for(size_t i = 0; i < Token::_nb_directives; i++) {
		if (Token::_directives[i][0] == name)
				return (true);	
	}
	return (false);	
}

bool isNumeric(const std::string &str) {
	size_t str_size;

	str_size = str.size();
	for (size_t i = 0; i < str_size; i++) {
		if (not std::isdigit(str[i]))
			return (false);
	}
	return (true);
}

void Lexer::skipSpcae() {
	while (this->_c and std::isspace(this->_c))
		readChar();
}

Token Lexer::getNextToken() {
	Token 		ret_token;
	std::string name;

	readChar();
	skipSpcae();
	switch (this->_c)
	{
		case ';':
			ret_token = Token(Token::COLON, ";");
			break;
		case '{':
			ret_token = Token(Token::LBRACE, "{");
			break;
		case '}':
			ret_token = Token(Token::RBRACE, "}");
			break;
		case 0:
			ret_token = Token(Token::TOKEN_EOF, "");
			break;
		default:
			name = getTokenName();
			if (isDirective(name))
				ret_token = Token(name, name);
			else if (name.empty())
				ret_token = Token(Token::TOKEN_EOF, Token::TOKEN_EOF);
			else
				ret_token = Token(Token::VALUE, name);
			break;
	}
	ret_token.setLineNum(this->_line_num);
	return (ret_token);
}
