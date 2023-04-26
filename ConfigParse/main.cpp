#include <iostream>
#include <vector>
#include <fstream>

std::string _load_config_file(const char *file_name_)
{
	std::fstream  		file;
	std::string			sstream;
	std::string			line;

	file.open(file_name_);
	if (!file.is_open())
		return (NULL);

	while (!file.eof())
	{
		getline(file, line);
		if (line.find('#') != std::string::npos )
			line.erase(line.begin() + line.find('#'), line.end());		
		sstream +=  line + "\n";
	}
	file.close();
	return (sstream);
}

bool is_keyword(char c)
{
	return (c == '{' || c == '}' || c == ';' || c == '\n' || c == ' ' );
}

void print_tokens(std::string &_data)
{
	std::string buff("");
	std::vector<std::string> tokens;
	size_t i = 0;
	char c;

	while (_data[i])
	{
		c = _data[i];
		if  (is_keyword(c))
		{
			if (!buff.empty())
				tokens.push_back(buff);
			if (c != ' ' )
				tokens.push_back(std::string(1,c ));
			buff.clear();
		}
		else
			buff += c;
		i++;
	}
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
		std::cout << "---: " << *it << std::endl;
}

int main()
{
 	std::string file_data = _load_config_file("/tmp/test_file");
	print_tokens(file_data);
}
