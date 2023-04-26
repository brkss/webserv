
class Token
{
	public:
		std::string _name;
		int			_type;
};

class Directive: public Token
{
		int			context;
}


