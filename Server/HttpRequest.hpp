#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP 

#include <map>
#include <iostream>

class HttpRequest {
	
	public:
		typedef std::map<std::string, std::string> headers_t; //Headers's Map Type

		enum REQUEST_STATE
		{
			HEADER_STATE,
			BODY_STATE,
			REQUEST_READY
		};

		enum BODY_TRANSFER {
			CHUNKED,
			CONTENT_LENGHT,
			UNSET
		};

		static const std::string GET;
		static const std::string POST;
		static const std::string DELETE;
		static const size_t		URI_MAX_LEN;

	private:
		REQUEST_STATE						_request_state;
		BODY_TRANSFER 						_transfer_type;
		std::string							_request_method;
		std::string							_request_path;
		std::string							_http_version;
		std::map<std::string, std::string>	_request_headers; 
		std::string							_request_data;	
		std::string							_request_body;
		
	public:
		HttpRequest();
		~HttpRequest();
		HttpRequest(const HttpRequest &request);
		HttpRequest &operator=(const HttpRequest &request);

		const std::string 	&getRequestMethod() const ;
		const std::string 	&getRequestPath()	const ;
		const std::string 	&getHttpVersion()	const ;
		const std::string&	getHeaderValue(const std::string &header); // Get header value by name,
																	   // returns "header" if not found 
	
		// #Request Proprty Getters 
		const headers_t		&getHeaders()			const;
		int					getRequestState()		const;
		int					getBodyTransferType()	const;
		std::string			&getRequestBody();
		std::string			&getRequestData();

		// #Parse Methods
		void				addRequestData(const std::string &data);
		void				parseHeaders(const std::string &headers);
		void				parseRequestLine(std::string &request_line);
		void				parse(std::string &request_header);

		// # Setters
		void				setRequestMethod(const std::string &method);
		void				setHttpVersion(const std::string &method);
		void				setRequestPath(const std::string &path)	;
		void 				setTransferType(BODY_TRANSFER type);
		void				setRequestState(REQUEST_STATE state);

		// #Dtata Trandfer Methods 
		void 				appendChunk(const std::string &chunk);
		void 				CheckTransferEncoding();
		void 				CheckTransferType();
		void 				CeckContentLength();
};


#endif /* HTTPREQUEST_HPP */
