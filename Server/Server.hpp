#pragma once
#include "../ft_irc.hpp"

class Channel;
class Client;

class Server
{
private:
    int                 		_serverSocket;
    int			        		_port;
    struct sockaddr_in		  	_serverAddr;
    std::string         		_password;
	std::vector<pollfd> 		_pollFds;
	std::vector<Channel *> 		_channels;
	//std::map <int, Client *> 	_clients;
	std::map<int , Client>		_MClient;

public:
    Server(std::string password, int port);
    ~Server();

    void	start();
	void	run();

	void	eventClient(Client *Client);

	std::vector<Channel *>	getServerChannels();
	Client 					getClientbyFd(int fd);

	void	handleMessage(std::string message, Client &client);
	void 	handleFirstConnection(int clientSocket);
	void	removeClient(int fd);

	int		checkNameValidity( std::string &name );
	void	handleRequestError( int error, Client &user ) const;
	bool	isClientAdded(int fd) const;
	bool	is_good_infos(std::string message, int clientSocket);
	


	// Commands
	void	joinCommand( std::string channelName, Client &client );
	void	partCommand( std::string channelName, Client &client );
	void	privMsgCommand( std::string command, Client &sender );
	
};
