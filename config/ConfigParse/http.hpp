#ifndef HTTP_HPP
# define HTTP_HPP 

class  Http
{
	private:


	public:
		Http();
		Http(const Http &parse);
		Http &operator=(const Http &parse);
		~Http();
		parseFile(const std::string &file_name);
};


#endif /* HTTP_HPP */
