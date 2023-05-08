



#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

class SocketWrapper {

	
	int sd; // socket descriptor 
	struct sockaddr_in address; // binding adress
	int connection;
	
	public:
		SocketWrapper(int domain, int type, int protocol, int port, unsigned long interface);
		void connect_to_network();
		int get_sd();
		int get_connection();
		struct sockaddr_in get_address();

};
