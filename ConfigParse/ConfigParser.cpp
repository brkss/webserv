#include "ConfigParser.hpp"

ConfigParse::ConfigParse() : _file_data(NULL)
{
}

ConfigParse::ConfigParse(const std::string &file_name_) :
{
	this->_file_name = file_name_;
	this->_file.open(file_name);
	if (!_file.isopen())
		throw ("Unable to open file");
	this->_file_data = this->load_file_data();
}

ConfigParse::ConfigParse(const ConfigParse& configparse_)
{
	this->_file_data = new std::string(*configparse_._file_data);
	this->_file_name = configparse_._file_name;
	this->_file = configparse_._file;
}

ConfigParse & ConfigParse::ConfigParse(const ConfigParse& configparse_)
{
	if (this != &configparse_)
	{
		delete this->_file_data;
		this->_file_data = new std::string(*configparse_._file_data);
		this->_file_name = configparse_._file_name;
		this->_file = configparse_._file;
	}
}

ConfigParse::~ConfigParse() 
{
	delete  this->_file_data;
}


std::string *ConfigParse::load_file_data(void)
{
	std::string file_buff;
	std::string 
		
	while(!_file.eof())
	{

	}	
}
