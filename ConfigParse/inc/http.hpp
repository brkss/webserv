#ifndef HTTP_HPP
# define HTTP_HPP 

#include <iostream> 
#include <map>

class  Http
{
	private:
		typedef std::map<std::string, std::string>::value_type pair;
		typedef std::map<short, std::string>::value_type page_pair_t;

	protected:
		std::map<std::string, std::string>	_cgi;
		std::map<short, std::string>		_error_pages;
		std::string 						_root;
		std::string 						_index;
		bool								_autoindex;
		size_t 								_client_max_body_size;
		size_t 								_client_request_timout; // timout for nly for a period between two successive read operations 

	public:
		Http();
		Http(const Http &parse);
		Http &operator=(const Http &parse);
		virtual ~Http();

		const std::map<short, std::string> &getErroPages() const ;
		const std::string &getRoot() const ;
		const std::string &getIndex() const ;
		const bool		  &getAutoIndex() const ;
		const size_t	  &getClientMaxBodySize() const ;

		void addErrorPaage(short error_code, const std::string &path);
		void setRoot(const std::string &value);
		void setIndex(const std::string &value);
		void setAutoIndex(bool value);
		void setClientMaxBodySize(size_t value);
		void setRequestTimout(size_t timeout);
		void addCgi(const pair  &cgi);


};

#endif /* HTTP_HPP */
