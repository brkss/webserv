


#include "ListeningSocket.hpp"
#include <unistd.h>



class Server {

	private:
		ListeningSocket *sock;
		int new_sock;
		char buffer[30000];
		std::string path;
	public:
		void accepter();
		void handler();
		void responder();
		void launch();
		Server(int domain, int type, int protocol, int port, unsigned long interface, int bklg);
		~Server();
};
