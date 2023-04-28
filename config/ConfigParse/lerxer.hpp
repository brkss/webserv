#ifndef LEXER_HPP
# define LEXER_HPP

class Lexer
{
	private:
		std::string _line;
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

};

#endif 
