#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP 

#include <map>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <fcntl.h> // O_RDWR flag
#include <unistd.h> // close() syscall

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
		std::string 						_query_string;
		std::string							_http_version;
		std::map<std::string, std::string>	_request_headers; 
		std::string							_request_data;	
		std::string							_request_body; // request body is now on disk at fd -->
		//std::ofstream						_data_file;
		std::string 						_data_filename;
		int 								_data_file_fd;
		int									_request_status_code; // is set on request error 
		size_t 								_request_body_size;

		//for debug 
		std::string 						_request_line;

	public:
		HttpRequest();
		~HttpRequest();
		HttpRequest(const HttpRequest &request);
		HttpRequest &operator=(const HttpRequest &request);

		const std::string 	&getRequestMethod() const ;
		const std::string 	&getRequestPath()	const ;
		const std::string 	&getHttpVersion()	const ;
		const std::string	&getQueryString() const; // get's ?query=test
		const std::string&	getHeaderValue(const std::string &header); // Get header value by name,
																	   // returns "header" if not found 
	
		// #Request Proprty Getters 
		const headers_t		&getHeaders()			const;
		int					getRequestState()		const;
		int					getBodyTransferType()	const;
		std::string			&getRequestBody();
		std::string			&getRequestData();
		int					getDataFileDescriptor();
		short				getStatusCode() const;
		const std::string 		&getRequestDataFilename() const;
		size_t 				getRequestBodySize()const;

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
		void				setQueryString(const std::string &url);
		void 				setStatusCode(int status);

		// #Dtata Trandfer Methods 
		void 				appendChunk(const std::string &chunk);
		void				validateHeaders();
		void 				CheckTransferEncoding();
		void 				CheckTransferType();
		void 				CeckContentLength();
		void 				openDataFile();

		void				writeChunkTofile(const std::string &data);

		const std::string & getRequestLine() const ; // for debug
};


#endif /* HTTPREQUEST_HPP */
