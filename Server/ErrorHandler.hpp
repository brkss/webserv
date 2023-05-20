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


#endif /* ERRORHANDLER_HPP */
