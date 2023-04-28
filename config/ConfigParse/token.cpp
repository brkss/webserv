#include "token.cpp"

Token()
{
}

Token(const Token& token_)
{
	(void) token_;
}

Token& operator=(const Token& token_)
{
	(void) token_;

	return (*this);
}

~Token()
{
}
