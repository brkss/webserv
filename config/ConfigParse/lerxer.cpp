
Lexer::Lexer(): _index(0), c_(0)  {}

Lexer::Lexer(const Lexer& lexer_)
{
	*this = lexer_;
}

Lexer::Lexer& operator=(const Lexer& lexer_)
{
	this->_c = lexer_._c;
	this->_line = lexer_._line;
	this->_index = lexer_._index;
}

Lexer::~Lexer() {}

void Lexer::readChar()
{
	if (this->_index < this->_line.size())
		this->_c = this->_line[_index];
	else
		this->_c = 0; // END of line
}

void Lexer::readWord()
{
	while (std::isalpha(this->_c))
		readChar();
}

char	getChar()
{
	return (this->_c);
}
