#include "Utils.hpp"

namespace Utils {
	std::vector<std::string> split(const std::string &str, const std::string &delim) {

	std::vector<std::string> ret;
	std::string  buff;
	size_t pos, last_pos = 0;
	while ((pos = str.find(delim, last_pos)) != std::string::npos) {
		buff = str.substr(last_pos, pos - last_pos);
		ret.push_back(buff);
		last_pos = pos + delim.size();
	}
	ret.push_back(str.substr(last_pos));
	return (ret);
}

};
