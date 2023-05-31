#include "Utils.hpp"

namespace Utils {
	std::vector<std::string> split(const std::string &str, const std::string &delim) {

	std::vector<std::string> ret;
	std::string  buff;
	size_t pos, last_pos = 0;
	while ((pos = str.find(delim, last_pos)) != std::string::npos) {
		buff = str.substr(last_pos, pos - last_pos);
		if (buff.size())
			ret.push_back(buff);
		last_pos = pos + delim.size();
	}
	ret.push_back(str.substr(last_pos));
	return (ret);
}

bool is_number(const std::string &str) {
	std::string::const_iterator it = str.begin();
	while (std::isdigit(*it))
		it++;
	return ( !str.empty() && it == str.end());

}

bool is_hex(const std::string &str) { 
	if (str.empty())
			return (false);
	for (size_t i = 0; i < str.size(); i++) {
		if (!std::isxdigit(str[i]))
			return (false);
	}
	return (true);
}

size_t hexStringToSizeT(const std::string &hex_str) {
	size_t ret_val;
	std::stringstream ss;
	ss << std::hex <<  hex_str;
	ss >> ret_val ;
	return (ret_val);
}

std::string trimSpaces(const std::string &str) {
	size_t start  = str.find_first_not_of(' ');
	size_t end  = str.find_last_not_of(' ');
	if (start == std::string::npos or end == std::string::npos)
		return ("");
	return (str.substr(start, end - start + 1));
}

};
