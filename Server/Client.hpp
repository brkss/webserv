#ifndef CLIENT_HPP
# define CLIENT_HPP 

class Client {
	private:
		int 				_server_fd; // server socket 
		int 				_connection_fd; // data socket 
		int					_address_len;
		struct socketaddr_in _client_address;


	public:
		Client();
		~Client();
		Client(const Client &client);
		Client &operator=(const Client &client);

		int getServerFd() const;
		struct socketaddr_in *getClientAddress() const;
		int *getAddressLen() const;
		int getConnectionFd() const ;

		void setConnectionFd(int fd);

};
#endif /* CLIENT_HPP */
