
std::string *_load_config_file(const char *file_name_)
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
		sstream +=  line;
	}
	file.close();
	return (new std::string(sstream));
}
