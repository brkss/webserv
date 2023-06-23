#ifndef LEXER_HPP
# define LEXER_HPP
			
#include "inc/token.hpp"

class Lexer
{
	private:
		std::string _line;
		size_t		_line_size;
		size_t		_index;
		char		_c;

	public:
		Lexer();		
		Lexer(const Lexer& lexer_);		
		Lexer& operator=(const Lexer& lexer_);		
		~Lexer();

	public:
		void	readChar();
		void 	readWord();
		char	getChar();
		void	setLine(const std::string &line);
		bool 	isDirective(const std::string &name) const;

};

#endif 
