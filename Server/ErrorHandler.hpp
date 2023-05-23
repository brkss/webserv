#ifndef ERRORHANDLER_HPP
# define ERRORHANDLER_HPP 

#include <exception>
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

class  ErrorMessage {
	public:
		static 	const char * ERROR_400 ;
		static 	const char * ERROR_414 ; 
		static 	const char * ERROR_505 ; 

};

#endif /* ERRORHANDLER_HPP */
