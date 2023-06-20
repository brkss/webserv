#include <iostream>
#include "ServerDrive.hpp"
#include <strings.h>
#include "ErrorHandler.hpp"
#include "Network.hpp"
#include <algorithm>
#include "Utils.hpp"
#include <unistd.h>

const std::string ServerDrive::HEADER_DELIM = "\r\n\r\n";
const std::string ServerDrive::CRLF			= "\r\n";

// Constructors 
ServerDrive::ServerDrive(Parse &conf): _config(conf),
											_virtual_servers(conf.getVirtualServers()),
											_fd_max(0),
											_client_timeout(5) {

		std::vector<Server>	&servers = this->_config.getVirtualServers(); 
		const int 					true_ = 1;
		int							sock_fd;

		FD_ZERO(&(this->_readset));
		FD_ZERO(&(this->_writeset));
		FD_ZERO(&(this->_listenset));
		for (Parse::v_iterator server = servers.begin(); server != servers.end(); server++)
		{
			sock_fd = Network::CreateSocket();
			setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &true_, sizeof(int));
			Network::BindSocket(sock_fd, server->getPort(), server->getAddress() );

			Network::ListenOnSocket(sock_fd);
			addSocketFd(sock_fd);
			FD_SET(sock_fd, &(this->_listenset)); {
				const std::string log = "Serving HTTP on " + server->getAddress() + " Port: " +  std::to_string(server->getPort());
				ConsoleLog::Specs(log) ;
			}
			server->setSocketFd(sock_fd);
		}
}

ServerDrive::ServerDrive(const ServerDrive &server) :_config(server._config),
													 _virtual_servers(server._virtual_servers){
}

ServerDrive::~ServerDrive() {
	for(std::vector<int>::iterator it = _server_fds.begin(); it != _server_fds.end(); it++)
		close(*it);
}

void sendErrorMessage(int fd, short error) {
	std::map<short, std::string> status_message;
	std::string message;

	std::string templat; 
	status_message[400] = "400 Bad request",
	status_message[408] = "408 Request timeout",
	status_message[411] = "411 Length required",
	status_message[414] = "414 Uri_too long",
	status_message[413] = "413 Payload too large",
	status_message[431] = "431 Request header fields too large",
	status_message[500] = "500 Internal server error",
	status_message[501] = "501 No timplemented",
	status_message[505] = "505 Http version not supported";
	message = status_message[error];
	std::string resp = "HTTP/1.1 " + message +  "\r\n\r\n";
	if (send(fd, resp.c_str(), resp.size() , 0) != (ssize_t ) resp.size()) {
		ConsoleLog::Error("Send error");
		perror(NULL);
		throw(ErrorLog("Send error message"));
	}
}

void ServerDrive::io_select(fd_set &read_copy, fd_set &write_copy) {
	
	struct timeval timout;
	int		select_stat;

	timout.tv_sec = 4;
	timout.tv_usec = 0;

	#if DEBUG
	ConsoleLog::Debug("Selecting socket fds ");
	#endif 
	select_stat =  select(this->_fd_max + 1, &read_copy, &write_copy, NULL, NULL);//&timout);
	if  (select_stat < 0)  {
		throw(RequestError(ErrorNumbers::_500_INTERNAL_SERVER_ERROR));
	}
}

void ServerDrive::addClient(Client client) {

	typedef std::map<int, Client>::value_type pair_t;

	int sock_fd = client.getConnectionFd();
	int server_fd = client.getServerFd();
	pair_t pair(sock_fd, client);
	client.setServer(getServerByFd(server_fd));
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
	client.setRequestTimout(time(NULL));
	if (bytes_recieved <= 0) {			// error on while reading 
		if (bytes_recieved == 0) { 		// Client closed connection
			Network::closeConnection(fd);
			CloseConnection(fd);
			ConsoleLog::Warning("EOF recieved closing...");
			return ;
		}
		else 
			throw(RequestError(ErrorNumbers::_500_INTERNAL_SERVER_ERROR));
	}
	client.saveRequestData(bytes_recieved); // PUSH BUFFER TO REQUEST MASTER BUFFER 
	CheckRequestStatus(client);
	#if DEBUG
	ConsoleLog::Warning("Receiving from Client: " + std::to_string(fd));
	#endif 
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
	if (request_data.empty() && (request.getRequestMethod() == HttpRequest::GET 
								|| request.getRequestMethod() == HttpRequest::DELETE) )
		request.setRequestState(HttpRequest::REQUEST_READY);
}

bool ServerDrive::unchunkBody(HttpRequest &request) {
	size_t		size_pos;
	size_t		chunk_size;
	std::string hex_sting;
	std::string &request_body = request.getRequestData();

	size_pos = request_body.find(ServerDrive::CRLF);
	if (size_pos == std::string::npos)
		return (false);

	hex_sting = request_body.substr(0, size_pos);
	if (!Utils::is_hex(hex_sting))
		throw (RequestError(ErrorNumbers::_400_BAD_REQUEST));

	chunk_size = Utils::hexStringToSizeT(hex_sting);
	if (chunk_size > (request_body.size() - (hex_sting.size() + CRLF.size() )))
		return false;									// READ REST OF CHUNK

	request_body = request_body.substr(size_pos + CRLF.size());
	if (chunk_size == 0) {
		request.setRequestState(HttpRequest::REQUEST_READY);
		request_body.clear(); return false;
	}
	std::string temp = request_body.substr(0, chunk_size);
	assert(chunk_size == temp.size());
	request.writeChunkTofile(temp);
	//request.appendChunk(temp);							// STORE CUNK IN BODY BUFFER
	request_body = request_body.substr(chunk_size + 2); // +2 EXPECTING CRLF AFTER CHUNK
	return (true);
}


bool	ServerDrive::getBody(HttpRequest &request) {

	const std::string	&length_str = request.getHeaderValue("Content-Length");
	std::string			&request_body = request.getRequestData();
	size_t				content_length =  std::atoi(length_str.c_str());
	size_t				bytes_left;

	bytes_left = content_length - request.getRequestBody().size();
	if (bytes_left <= request_body.size()) {
		request.writeChunkTofile(request_body.substr(0, bytes_left));
		request.appendChunk(request_body.substr(0, bytes_left));
		assert(request.getRequestBody().size() == content_length);
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
		client_request.openDataFile();
		if (client_request.getBodyTransferType() == HttpRequest::CHUNKED) {
			while (!client_request.getRequestData().empty() && unchunkBody(client_request)) {
			}
		}
		else if (client_request.getBodyTransferType() == HttpRequest::CONTENT_LENGHT) {
			if (getBody(client_request))  {
				client_request.setRequestState(HttpRequest::REQUEST_READY);
			}
		}
		else
			throw(RequestError(ErrorNumbers::_411_LENGTH_REQUIRED));
	}
	if (client_request.getRequestState() == HttpRequest::REQUEST_READY)  {
		FD_SET(client.getConnectionFd() , &(this->_writeset)); 
		const Server & client_server = getServerByName(client_request.getHeaderValue("Host")); // Supports virtual hosting 
		client.setServer(client_server);
		#if DEBUG 
		ConsoleLog::Debug("server handeling the request : " + client.getServer().getServerName());
		#endif

		return ;
		// TESTING DATA TRANSFER
		const std::string out_file_name = client.getServer().getRoot() +  client.getServer().getUploadStore() + "/oupload"  + std::to_string(client.getConnectionFd());
		std::ofstream ofs(out_file_name);
		if (ofs.is_open())
			ofs << client.getRequest().getRequestBody() ;
		else {
			throw(RequestError(ErrorNumbers::_500_INTERNAL_SERVER_ERROR));
		}
		ConsoleLog::Specs("Data uploaded to :" + out_file_name);
		// should add log error 
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

	Client &client				= getClient(fd) ;
	time_t last_event			= client.getClientRequestTimeout();
	time_t config_timeout 		= this->_client_timeout; // PS: READ HEADER 
	time_t elapsed				= time (NULL) - last_event;

	if (elapsed >= config_timeout) {
		ConsoleLog::Warning("Connection Timout Closing ... ");
		throw(RequestError(ErrorNumbers::_408_REQUEST_TIMEOUT));
	}
}

bool ServerDrive::ClientError(int fd) {
	Client &client				= getClient(fd) ;
	HttpRequest &request = client.getRequest();
	short 	error				= client.getRequestStatus();
	if (error != 0) {
		sendErrorMessage(fd, error);
		const std::string log = "[Request]: " + request.getRequestLine() + " (" +  std::to_string(error) + ")" ;
		CloseConnection(client.getConnectionFd());
		ConsoleLog::Error(log);
		return (true);
	}
	return (false);
}

char *moveToHeap(const std::string &resp) {
	const char *r = resp.c_str();
	char *resp_copy = new char [resp.size()];
	memmove(resp_copy, r, resp.size());
	return (resp_copy);
}

void PrepareResponse(Client &client)  {


	Handler	handler(client);
	Response response(handler.getBody(), handler.getType(), handler.getSize(), handler.getStatus(), handler.getFD());
	client.setResponseObj(response);
	//std::string resp = response.generateResponse();
}

void ServerDrive::SendResponse(Client &client) {


	Response &client_response	= client.getResponseObj();
	short 	client_fd			= client.getConnectionFd();
	size_t	socket_buffer_size	= Network::getSocketBufferSize(client_fd, SO_RCVBUF);
	const std::string &response	= client_response.getResponseChunk(socket_buffer_size);
	size_t	response_size		= response.size();
	bool	close_connection	= false;
		
	if (response_size == 0) 
			close_connection = true;
	if (int ss = send(client_fd, response.c_str(), response_size, 0) != (ssize_t ) response_size) {
			close_connection = true;
		#if DEBUG
		client.getRequest().setStatusCode(0xde);//eadbeef
		perror("Send error :");
		ConsoleLog::Warning("Send Error: failed to  writre data to socket !");
		#endif 
	}
	#if DEBUG
	ConsoleLog::Debug("Response Portion  Sent!" );
	#endif 
	if (close_connection) {
		#if DEBUG
		std::string debug_log = "::WebServ Response Sent!. Closing fd :...";
		debug_log = debug_log + std::to_string(client_fd);
		ConsoleLog::Debug(debug_log);
		#endif 
		log(client);
		CloseConnection(client_fd);
	}
}

void ServerDrive::log(Client &client) {
	if (client.getRequest().getStatusCode()) {
		ConsoleLog::Error("[Send Error]");
		return ;
	}
	Response 	&response_obj = client.getResponseObj();
	const std::string log = "[Response]: " + client.getRequest().getRequestLine() + " (" +  response_obj.getStatusCode() + ")";
	ConsoleLog::Specs(log);
}

void ServerDrive::eventHandler(fd_set &read_copy, fd_set &write_copy) {

	int fd_max = this->_fd_max;

	for (int fd = 3; fd <=  fd_max; fd++) {
		try { 
			if (FD_ISSET(fd, &write_copy) && not ClientError(fd)) {					// response 
				Client &client = getClient(fd);
				if (not client.ResponseReady())
					PrepareResponse(client);
				#if DEBUG
				ConsoleLog::Debug("Sending response ...");
				#endif 
				SendResponse(client);
			}
			else if (FD_ISSET(fd, &read_copy)) {
				if (FD_ISSET(fd, &this->_listenset)) 
					addClient(Network::acceptConnection(fd));	// new connection 
				else 											// read request 
					readRequest(fd);
			}
			else if (!FD_ISSET(fd, &this->_listenset) 
					&& FD_ISSET(fd, &(this->_readset)) 
					&& !FD_ISSET(fd, &this->_writeset)) // CLIENT SHOULD BE CHECKED FOR TIMEOUT
				checkClientTimout(fd);	

		} catch (const RequestError &error)  {
			#if DEBUG
			ConsoleLog::Error("Request Error: status code : " + std::to_string(error.getErrorNumber()));
			#endif
			FD_CLR(fd, &(this->_readset));
			FD_SET(fd, &this->_writeset);		// select before response
			getClient(fd).setRequestStatus(error.getErrorNumber());
		}
	}
}

void ServerDrive::run() {
	fd_set read_copy;
	fd_set write_copy;

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
	else {
		throw(ErrorLog("BUG: Potential Server error"));
	}
}

const Server &ServerDrive::getServerByName(const std::string &host_name) {
	typedef std::vector<Server>::const_iterator cv_iterator;

	for (cv_iterator it = this->_virtual_servers.begin(); it != this->_virtual_servers.end(); it++) {
		if (it->getServerName() == host_name) 
			return (*it);
	}
	return (*this->_virtual_servers.begin());
}

const Server &ServerDrive::getServerByFd(int fd) {
	typedef std::vector<Server>::const_iterator cv_iterator;

	for (cv_iterator server = this->_virtual_servers.begin(); server != this->_virtual_servers.end(); server++) {
		if (server->getSocketFd() == fd) 
			return (*server);
	}
	return (*this->_virtual_servers.begin());
}

