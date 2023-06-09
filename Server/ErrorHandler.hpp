#ifndef ERRORHANDLER_HPP
# define ERRORHANDLER_HPP 

#include <exception>
#include <iostream>
#include "Colors.h"

class ErrorLog : public std::exception {
	private:
		const  char *_error;
	public:
		ErrorLog(const char * error) : _error(error) {
		}
		ErrorLog(const ErrorLog &error_log) : _error(error_log._error) { } const char *what() const throw() {
			return  this->_error;
		}
};

class ConsoleLog { 

	public:
		static void Error(const std::string &error) {
			std::cerr << Color::RED << error << Color::DEFAULT << std::endl;
		}
		static void Warning(const std::string &warnig) {
			std::cerr << Color::YELLOW << warnig << Color::DEFAULT << std::endl;
		}
		static void Debug(const std::string &warnig) {
			std::cerr << Color::YELLOW << warnig << Color::DEFAULT << std::endl;
		}
		static void Specs(const std::string &spec) {
			std::cerr << Color::GREEN << spec << Color::DEFAULT << std::endl;
		}

};


class  ErrorMessage {
	public:
		static 	const char * ERROR_400 ;
		static 	const char * ERROR_414 ; 
		static 	const char * ERROR_505 ; 

};

class  ErrorNumbers {
	public:
		static const short	_400_BAD_REQUEST;
		static const short 	_408_REQUEST_TIMEOUT; 
		static const short 	_411_LENGTH_REQUIRED; 
		static const short 	_414_URI_TOO_LONG;
		static const short	_413_PAYLOAD_TOO_LARGE;
		static const short 	_431_REQUEST_HEADER_FIELDS_TOO_LARGE;	
		static const short	_500_INTERNAL_SERVER_ERROR;
		static const short	_501_NOT_IMPLEMENTED;	
		static const short	_505_HTTP_VERSION_NOT_SUPPORTED;
};

class RequestError {
	private:
		int _error_number;

	public:
		RequestError(int error_number) : _error_number(error_number) {
		}
		int getErrorNumber() const  {
			return (this->_error_number);
		}
};

#endif /* ERRORHANDLER_HPP */
