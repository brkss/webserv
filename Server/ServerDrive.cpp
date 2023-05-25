#include <iostream> 
#include "ServerDrive.hpp"
#include <strings.h>
#include "ErrorHandler.hpp"
#include "Network.hpp"
#include <algorithm>
#include "Utils.hpp"

const std::string ServerDrive::HEADER_DELIM = "\r\n\r\n";
const std::string ServerDrive::CRLF = "\r\n";

void send_test_response(int fd) {

		const char * resp =  "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 52\nContent-Type: text/html\nConnection: Closed\n\n<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>\n";

	if (send(fd, resp, strlen(resp), 0) != (ssize_t ) strlen(resp))
		throw(ErrorLog("Send error"));
}

ServerDrive::ServerDrive(const Parse &conf): _config(conf), _fd_max(0) {
		
	this->_client_timeout = 5; // (in seconds should be pulled from config);

	const std::vector<Server>	&servers = this->_config.getServers(); 
	const int 					true_ = 1;
	int							sock_fd;

	FD_ZERO(&(this->_readset));
	FD_ZERO(&(this->_writeset));
	FD_ZERO(&(this->_listenset));

	for (Parse::cv_itereator cit = servers.begin(); cit != servers.end(); cit++)
	{
			sock_fd = Network::CreateSocket();
			setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &true_, sizeof(int));
			Network::BindSocket(sock_fd, cit->getPort(), cit->getAddress() );
			Network::ListenOnSocket(sock_fd);
			addSocketFd(sock_fd);
			FD_SET(sock_fd, &(this->_listenset));
			std::cerr << "Server Listening On " <<  cit->getAddress() << "[" << cit->getPort() << "]" << std::endl;
	}
}

ServerDrive::ServerDrive(const ServerDrive &server) : _config(server._config) {
	(void) server;
}

ServerDrive::~ServerDrive() {
	for(std::vector<int>::iterator it = _server_fds.begin(); it != _server_fds.end(); it++)
		close(*it);
}

void ServerDrive::io_select(fd_set &read_copy, fd_set &write_copy) {
	struct timeval timout;
	int		select_stat;

	timout.tv_sec = 4;
	timout.tv_usec = 0;
	select_stat =  select(this->_fd_max + 1, &read_copy, &write_copy, NULL, &timout);
	if  (select_stat < 0)  {
		throw(ErrorLog("Error: Select failed"));
	}
}

void ServerDrive::addClient(Client client) {

	typedef std::map<int, Client>::value_type pair_t;

	int sock_fd = client.getConnectionFd();
	pair_t pair(sock_fd, client);
	this->_clients.insert(pair);
	addSocketFd(sock_fd);
}

void ServerDrive::addSocketFd(int fd) {
	this->_server_fds.push_back(fd);
	FD_SET(fd, &(this->_readset)); 
}

void ServerDrive::readRequest(int fd) {
	Client &client				= getClient(fd) ;
	char *buffer				= client._buffer;
	size_t client_buffer_size	= sizeof(client._buffer);
	size_t bytes_recieved;

	bzero(buffer, client_buffer_size);
	bytes_recieved = read(client.getConnectionFd(), buffer, client_buffer_size - 1);
	client.setRequestTimout(time(NULL)); // MARK 
	std::cerr << "Receving from client : " << client.getConnectionFd()  << std::endl;

	if (bytes_recieved <= 0)  // error on while reading 
	{ 	
		if (bytes_recieved == 0) // Client closed connection
		{     
			Network::closeConnection(fd);
			CloseConnection(fd);
			//FD_CLR(fd, &this->_readset);
			//FD_CLR(fd, &this->_writeset);
			//this->_server_fds.erase(std::find(this->_server_fds.begin(), this->_server_fds.end(), fd));
			std::cerr << "EOF recieved closing..." << std::endl;
			return ;
		}
		else 
			throw(ErrorLog("Error: unable to read form sock"));
	}
	client.saveRequestData(bytes_recieved); // PUSH BUFFER TO REQUEST MASTER BUFFER 
	CheckRequestStatus(client);


}

void ServerDrive::getHeader(HttpRequest &request)  {
	std::string &request_data = request.getRequestData();
	size_t header_pos = request_data.find(HEADER_DELIM);

	if (header_pos != std::string::npos) {
		std::string header = request_data.substr(0, header_pos);
		std::string rest = request_data.substr(header_pos + HEADER_DELIM.size());
		request.parse(header);
		request_data = rest;
	}
	if (request_data.empty() && request.getRequestMethod() == "GET") 
		request.setRequestState(HttpRequest::REQUEST_READY);
}

bool ServerDrive::unchunkBody(HttpRequest &request) {
	std::string &request_body = request.getRequestData();
	std::string hex_sting;

	size_t	size_pos;
	size_t	chunk_size;

	size_pos = request_body.find(ServerDrive::CRLF);
	if (size_pos == std::string::npos)
		return (false);

	hex_sting = request_body.substr(0, size_pos);
	if (!Utils::is_hex(hex_sting))
		throw (ErrorLog(ErrorMessage::ERROR_400));
			

	chunk_size = Utils::hexStringToSizeT(hex_sting);
	if (chunk_size > (request_body.size() - (hex_sting.size() + CRLF.size() )))
		return false; // READ REST OF CHUNK

	request_body = request_body.substr(size_pos + CRLF.size());
	if (chunk_size == 0) {
		request.setRequestState(HttpRequest::REQUEST_READY);
		request_body.clear(); return false;
	}

	std::string temp = request_body.substr(0, chunk_size);
	//std::cout << "body size: " << temp.size() << std::endl;
	//std::cout << "chunk size: " << chunk_size << std::endl;
	assert(chunk_size == temp.size());

	request.appendChunk(temp); // STORE CUNKS IN BODY BUFFER
	try {
	request_body = request_body.substr(chunk_size + 2); // +2 EXPECTING CRLF AFTER CHUNK

	}catch (const std::out_of_range& oor) {
		     std::cerr << "Out of Range error: " << oor.what() << '\n';
			 exit(1);
			   }
	return (true);
}

bool	ServerDrive::getBody(HttpRequest &request) {

	const std::string &length_str = request.getHeaderValue("Content-Length");
	std::string &request_body = request.getRequestData();
	size_t  content_length =  std::atoi(length_str.c_str());
	size_t bytes_left;

	bytes_left = content_length - request.getRequestBody().size();

	if (bytes_left <= request_body.size()) {
		request.appendChunk(request_body.substr(0, bytes_left));
		//std::cout << "Current BodySize : " << request.getRequestBody().size() << std::endl;
		//std::cout << "Content-Length : " << content_length << std::endl;
		assert(request.getRequestBody().size() ==  content_length);
		return (true);
	}
	return (false);
}

void ServerDrive::CheckRequestStatus(Client &client) {
	HttpRequest &client_request = client.getRequest();

	if (client_request.getRequestState() == HttpRequest::HEADER_STATE) {
		getHeader(client_request);
	}
	if (client_request.getRequestState() == HttpRequest::BODY_STATE)  {
		if (client_request.getBodyTransferType() == HttpRequest::CHUNKED) {
			while (!client_request.getRequestData().empty() && unchunkBody(client_request)) {

			}
		}
		else if (client_request.getBodyTransferType() == HttpRequest::CONTENT_LENGHT) {
			if (getBody(client_request))  {
				client_request.setRequestState(HttpRequest::REQUEST_READY);
			}
		}
	}
	if (client_request.getRequestState() == HttpRequest::REQUEST_READY)  {
		FD_SET(client.getConnectionFd() , &(this->_writeset)); 
		
		// TESTING DATA TRANSFER
		const std::string out_file_name = "./tests/out_file" + std::to_string(client.getConnectionFd());
		std::ofstream ofs(out_file_name);
		if (ofs.is_open())
			ofs << client.getRequest().getRequestBody() ;
		else
			throw(ErrorLog("can't open file for writting" ));
	}
}

void ServerDrive::CloseConnection(int fd) {
	Network::closeConnection(fd);
	FD_CLR(fd, &this->_readset);
	FD_CLR(fd, &this->_writeset);
	this->_server_fds.erase(std::find(this->_server_fds.begin(), this->_server_fds.end(), fd));
	this->_clients.erase(fd);
}

void ServerDrive::checkClientTimout(int fd) {
	
	std::cerr << "checking  timout for fd :"  << fd << std::endl;
	Client &client				= getClient(fd) ;
	time_t last_event			= client.getClientRequestTimeout();
	time_t config_timeout 		= this->_client_timeout; // PS: READ HEADER 
	time_t elapsed				= time(NULL) - last_event;
	
	if (elapsed >= config_timeout)
	{
		std::cout << "Connection Timout Closing ... " << std::endl;
		CloseConnection(fd);		
	}
	// GOOD BOY :)
}

void ServerDrive::eventHandler(fd_set &read_copy, fd_set &write_copy) {

	int fd_max = this->_fd_max;

	for (int fd = 3; fd <=  fd_max; fd++) {
		if (FD_ISSET(fd, &write_copy)) { 		// response 
			send_test_response(fd);
			CloseConnection(fd);
			std::cerr << "[" << fd << "]" << "Response sent. Closing ..." << std::endl;
		}
		else if (FD_ISSET(fd, &read_copy)) {
			if (FD_ISSET(fd, &this->_listenset)) 
				addClient(Network::acceptConnection(fd));	// new connection 
			else { 											// read request 
				std::cerr << "reading request for : " << fd << std::endl;
				readRequest(fd);
			}
		}
		else if (!FD_ISSET(fd, &this->_listenset) && FD_ISSET(fd, &(this->_readset))) { // SHOULD BE CHECKED FOR TIMEOUT
			checkClientTimout(fd);	
		}
	}
}

void ServerDrive::run() {
	fd_set read_copy;
	fd_set write_copy;

	std::cerr << "Server Running ..." << std::endl;
	while (1) {
		read_copy = this->_readset;
		write_copy = this->_writeset;
		this->_fd_max = *std::max_element(this->_server_fds.begin(), this->_server_fds.end());
		io_select(read_copy, write_copy);
		eventHandler(read_copy, write_copy);
	}
}

Client &ServerDrive::getClient(int fd) {
	clinetiterator_t it = this->_clients.find(fd);
	if (it != this->_clients.end())
		return (it->second);
	else 
	{
		std::cout << "fd : " << fd << std::endl; 
		throw(ErrorLog("BUG: Potential   Server  error"));
	}
}
