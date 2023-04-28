#ifndef PARSE_HPP
# define PARSE_HPP 

class  Parse
{
	private:

	public:
		Parse();
		Parse(const Parse &parse);
		Parse &operator=(const Parse &parse);
		~Parse();
		parseFile(const std::string &file_name);
};


#endif /* PARSE_HPP */
