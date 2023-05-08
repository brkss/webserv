


#include "ListeningSocket.hpp"
#include <sys/socket.h>


ListeningSocket::ListeningSocket(int domain, int type, int protocol, int port, unsigned long interface, int bklg): 
	SocketWrapper(domain, type, protocol, port, interface ){

		backlog = bklg;
		this->start_listening();
}

void ListeningSocket::start_listening(){
	listening = listen(get_sd(), this->backlog);
	if (listening < 0){
		perror("Can't listen !");
		exit(EXIT_FAILURE);
	}
}
