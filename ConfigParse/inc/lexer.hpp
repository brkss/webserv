#ifndef LEXER_HPP
# define LEXER_HPP
			
#include "token.hpp"

class Lexer
{
	private:
		std::string _line;
		size_t		_line_size;
		size_t		_index;
		size_t		_line_num;
		char		_c;


	public:
		Lexer();		
		Lexer(const Lexer& lexer_);		
		Lexer& operator=(const Lexer& lexer_);		
		~Lexer();

	public:
		std::string getTokenName();
		Token 		getNextToken();
		char		getChar();
		void		readChar();
		void 		readWord();
		void		setLine(const std::string &line, size_t line_num);
		bool 		isDirective(const std::string &name) const;
		void 		skipSpcae();

};

#endif 
