#pragma once
#include "ft_irc.hpp"

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
	std::vector <Channel> 	_channels;
	//std::map <int, Client *> 	_clients;
	std::map<int , Client>		_MClient;

public:
    Server(std::string password, char *port);
    ~Server();

    void	start();
	void	run();

	void	eventClient(Client *Client);

	std::vector<Channel>	getServerChannels();
	Client 					getClientbyFd(int fd);

	void	handleMessage(std::string message, Client &client);
	void	handleFirstConnection(int clientSocket);
	void	removeClient(int fd);

	int		checkNameValidity( std::string &name );
	void	handleRequestError( int error, Client &user ) const;


	// Commands
	void	joinCommand(std::string channelName, Client &client);
	
};

