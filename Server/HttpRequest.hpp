#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP 

#include <map>
#include <iostream>

class HttpRequest {
	
	public:
		enum REQUEST_STATE
		{
			HEADER_STATE,
			BODY_STATE 
		};
		typedef std::map<std::string, std::string> headers_t; // MAP OF HEADERS


	private:
		REQUEST_STATE						_request_state;
		std::string							_request_method;
		std::string							_request_path;
		std::string							_http_version;
		std::map<std::string, std::string>	_request_headers; 
		std::string							_request_data;	
		
	
	public:

		HttpRequest();
		~HttpRequest();
		HttpRequest(const HttpRequest &request);
		HttpRequest &operator=(const HttpRequest &request);

		const std::string 	&getRequestMethod() const ;
		const std::string 	&getRequestPath()	const ;
		const std::string 	&getHttpVersion()	const ;
		const headers_t		&getHeaders()		const ; 
		int					getRequestState() const;

		void				setRequestState(REQUEST_STATE state);
		void				addRequestData(const std::string &data);
		std::string			&getRequestData();

};


#endif /* HTTPREQUEST_HPP */
