#ifndef TOKEN_HPP
# define TOKEN_HPP

class Token
{
	private:
		const std::string _name;
		const std::string _type;

	public:
		// Modifiers 
		static const std::string HTTP;
		static const std::string SERVER;
		static const std::string LOCATION;
		
		// Directives
	   	static const std::string ROOT;
	   	static const std::string LISTEN;
	   	static const std::string SERVER_NAME;
	   	static const std::string ERROR_LOG;
	   	static const std::string CLIENT_MAX_BODY_SIZE;
		static const std::string RETURN;
		static const std::string ROOT;
		static const std::string AUTOINDEX;
		static const std::string INDEX;


	public:
		Token();		
		Token(const Token& token_);		
		Token& operator=(const Token& token_);		
		~Token();		
};

#endif 
