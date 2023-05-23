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

	timout.tv_sec = 5;
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
	std::cerr << "Receving from client : " << client.getConnectionFd()  << std::endl;

	if (bytes_recieved <= 0)  // error on while reading 
	{ 	
		if (bytes_recieved == 0) // Client closed connection
		{     
			Network::closeConnection(fd);
			FD_CLR(fd, &this->_readset);
			FD_CLR(fd, &this->_writeset);
			this->_server_fds.erase(std::find(this->_server_fds.begin(), this->_server_fds.end(), fd));
			std::cout << "EOF recieved closing..." << std::endl;
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
}

void ServerDrive::unchunkBody(HttpRequest &request) {
	std::string &request_body = request.getRequestData();
	std::string hex_sting;

	size_t	size_pos;
	size_t	chunk_size;
	size_pos = request_body.find(ServerDrive::CRLF);

	if (size_pos == std::string::npos)
		throw (ErrorLog(ErrorMessage::ERROR_400));

	hex_sting = request_body.substr(0, size_pos);
	if (!Utils::is_hex(hex_sting))
		throw (ErrorLog(ErrorMessage::ERROR_400));
	chunk_size = Utils::hexStringToSizeT(hex_sting);
	request_body = request_body.substr(size_pos + CRLF.size());

	if (chunk_size == 0) {
		request.setRequestState(HttpRequest::REQUEST_READY);
		request_body.clear(); return ;
	}
	request.appendChunk(request_body.substr(0, chunk_size)); // STORE CUNKS IN BODY BUFFER
	request_body = request_body.substr(chunk_size + 2); // +2 EXPECTING CRLF AFTER CHUNK
}

void ServerDrive::CheckRequestStatus(Client &client) {
	HttpRequest &client_request = client.getRequest();

	if (client_request.getRequestState() == HttpRequest::HEADER_STATE) {
		getHeader(client_request);
	}
	if (client_request.getRequestState() == HttpRequest::BODY_STATE)  {
		if (client_request.getBodyTransferType() == HttpRequest::CHUNKED) {
			while (!client_request.getRequestData().empty()) {
				unchunkBody(client_request);
			}
		}
		else if (client_request.getBodyTransferType() == HttpRequest::CONTENT_LENGHT) {
			std::cout <<  "Body : \n" << client_request.getRequestData() << std::endl;
		}
	}
}

void ServerDrive::CloseConnection(int fd) {
	Network::closeConnection(fd);
	FD_CLR(fd, &this->_readset);
	FD_CLR(fd, &this->_writeset);
	this->_server_fds.erase(std::find(this->_server_fds.begin(), this->_server_fds.end(), fd));
	this->_clients.erase(fd);
}

void ServerDrive::eventHandler(fd_set &read_copy, fd_set &write_copy) {

	int fd_max = this->_fd_max;

	for (int fd = 0; fd <=  fd_max; fd++) {
		if (FD_ISSET(fd, &write_copy)) { 		// response 
			send_test_response(fd);
			CloseConnection(fd);
			std::cerr << "Response sent. Closing ..." << std::endl;
		}
		else if (FD_ISSET(fd, &read_copy)) {
			if (FD_ISSET(fd, &this->_listenset)) 
				addClient(Network::acceptConnection(fd));	// new connection 
			else { 											// read request 
				FD_SET(fd, &(this->_writeset)); 
				readRequest(fd);
			}
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
		throw(ErrorLog("BUG: Potential   Server  error"));
}

