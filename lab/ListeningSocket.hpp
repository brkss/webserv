



#include "Socket.hpp"



class ListeningSocket : public SocketWrapper {

	int backlog;
	int listening;
	public:
		ListeningSocket(int domain, int type, int protocol, int port, unsigned long interface, int bklg);
		void start_listening();


};
