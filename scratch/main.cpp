#include "Server.hpp"
#include <iostream>
#include <string>
#include <fstream>

#define SERVER_SCOPE 1
#define LOCATION_SCOPE 2

bool set_scope(const std::string &line, short &scope)
{
	if (line.find("server:") == 0 and scope != 1)
		scope = 1;
	else if (line.find("location:") == 1)
		scope =  2;
	else 
		return (0);
	return (1);
}

bool check_indent(const std::string &line_, short &scop_, short line_num_)
{
	static int line_num;

	if (line_.find_first_not_of(" \t\n") != scop_  && !set_scope(line_, scop_))
	{
		std::cerr <<  "Unexpected indent at line: " << line_num_ << std::endl;
		return (false);
	}
	return (true);
}

//Server *get_server_config(std::fstream file)
//{
//	short  scope  = 1; 
//	std::string line;
//	Server server;
//
//	while (!file.eof())
//	{
//	 	std::getline(file, line);
//		if (line.find_first_not_of(" \t\n") == std::string::npos)
//			continue;
//
//		if (line.find_first_not_of(" \t\n") != scope)
//		{
//			std::cerr <<  "Unexpected indent at line: " << line_num << std::endl;
//			break ;
//		}
//	}
//	
//}

void test_parse(std::string file_name_)
{
	std::fstream  		file;
	std::string			line;
	short int			scope = 0; 
	short int			line_num = 0; 

	file.open(file_name_);
	if (!file.is_open())
		throw "Unable to open file";

	while (!file.eof())
	{
	 	std::getline(file, line);
		line_num++;
		if (line.find_first_not_of(" \t\n") == std::string::npos)
			continue;
		if (!check_indent(line,scope, line_num))
			break;
		std::cout <<  "--: " <<   line << std::endl;
		set_scope(line, scope);
	}
	file.close();
}

int main(int ac, char **av)
{
	Server test_server;

	if (ac != 2)
	{
		std::cout << "USE: ./server <file-name>"	 << std::endl;
		return (1);
	}
	test_parse(av[1]);
	return 0;
}
