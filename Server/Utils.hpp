#ifndef UTILS_HPP
# define UTILS_HPP 

#include <vector>
#include <algorithm>
#include <string>
//#include <ios>
#include <sstream>

namespace Utils {
	std::vector<std::string> split(const std::string &str, const std::string &delim);
	std::string trimSpaces(const std::string &str);
	bool is_number(const std::string &str);
	bool is_hex(const std::string &str);
	size_t hexStringToSizeT(const std::string &hex_str);
}
#endif /* UTILS_HPP */
