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
		static const std::string GET;
		static const std::string POST;
		static const std::string DELETE;
		static const size_t		URI_MAX_LEN;

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
		void				parse(std::string &header);
		void				parseRequestLine(std::string &request_line);
		void				parseHedears(const std::string &headers);

		void				setRequestMethod(const std::string &method);
		void				setRequestPath(const std::string &path)	;
		void				setHttpVersion(const std::string &method);


};


#endif /* HTTPREQUEST_HPP */
