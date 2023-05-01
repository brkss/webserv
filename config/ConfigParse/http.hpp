#ifndef HTTP_HPP
# define HTTP_HPP 

#include <iostream> 

class  Http
{
	protected:
		std::string 		_erro_log;
		std::string 		_root;
		std::string 		_index;
		bool				_autoindex;
		size_t 				_client_max_body_size;

	public:
		Http();
		Http(const Http &parse);
		Http &operator=(const Http &parse);
		virtual ~Http();

		const std::string &getErroLog() const ;
		const std::string &getRoot() const ;
		const std::string &getIndex() const ;
		const bool		  &getAutoindex() const ;
		const size_t	  &getClientMaxBodySize() const ;

};

#endif /* HTTP_HPP */
