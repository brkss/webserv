#ifndef  CONFIGPARSE_HPP
# define  CONFIGPARSE_HPP

#include <iostram>
#include <fstream>


class ConfigParse
{
	private:
		std::string			_file_name;
		std::string			*_file_data;
		std::ifstream		_file;
		
		ConfigParse();
	public:

		ConfigParse ConfigParse(const ConfigParse& configparse_);
		ConfigParse &operator=(const ConfigParse& configparse_);
		~ConfigParse();
		ConfigParse(const std::string &file_name_);

	private:
		std::string *load_file_data(void); 
		
};



#endif /* CONFIGPARSE_HPP */
