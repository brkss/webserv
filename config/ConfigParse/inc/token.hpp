#ifndef TOKEN_HPP
# define TOKEN_HPP

#include <string>

class Token
{
	private:
		std::string _value;
		std::string _type;
		size_t		_line_num;

	public:
		// Modifiers 
		static const std::string HTTP;
		static const std::string SERVER;
		static const std::string LOCATION;
		// Directives
	   	static const std::string ROOT;
	   	static const std::string LISTEN;
	   	static const std::string SERVER_NAME;
	   	static const std::string ERROR_PAGE;
	   	static const std::string CLIENT_MAX_BODY_SIZE;
	   	static const std::string CLIENT_REQUEST_TIMOUT;
		static const std::string LIMIT_EXCEPT;
		static const std::string RETURN;
		static const std::string AUTOINDEX;
		static const std::string INDEX;
		static const std::string TOKEN_EOF;
		static const std::string CGI;
		static const std::string COLON;
		static const std::string LBRACE;
		static const std::string RBRACE;
		static const std::string VALUE;
		static const std::string INT_VALUE;
		static const std::string UNDEFINED;
		static const std::string UPLOAD_STORE;

		static const std::string _directives[14][2];
		static const size_t		 _nb_directives;

	public:
		Token();		
		Token(const Token& token_);		
		Token& operator=(const Token& token_);		
		Token(const std::string &type, const std::string &value);
		~Token();

		const std::string &getTokenType() const;
		const std::string &getTokenValue() const;
		const size_t &getLineNum() const;

		bool isGlobalDirective() const ;
		void setLineNum(size_t  line_num);
};

#endif /* TOKEN_HPP */
