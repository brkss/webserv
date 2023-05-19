#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP 

#include <map>
#include <iostream>

class HttpRequest {
	
	private:
		bool	   							_request_ready;
		std::string							_request_method;
		std::string							_request_path;
		std::string							_http_version;
		std::map<std::string, std::string>	_request_headers; 

		std::string							_request_data;	
		
	
	public:
		typedef std::map<std::string, std::string> headers_t; // MAP OF HEADERS

		HttpRequest();
		~HttpRequest();
		HttpRequest(const HttpRequest &request);
		HttpRequest &operator=(const HttpRequest &request);

		const std::string 	&getRequestMethod() const ;
		const std::string 	&getRequestPath()	const ;
		const std::string 	&getHttpVersion()	const ;
		const headers_t		&getHeaders()		const ; 

		bool 				isReady()			const;

		void				addRequestData(const std::string &data);

};


#endif /* HTTPREQUEST_HPP */
