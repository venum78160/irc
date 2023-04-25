#pragma once
#include "../ft_irc.hpp"

class Channel;
class Client;

class Server
{
private:
	int				 			_serverSocket;
	int							_port;
	struct sockaddr_in		  	_serverAddr;
	std::string		 			_password;
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
	Client				  &getClientByNickname(std::string nickname);

	void		handleMessage(std::string message, Client &client);
	void 		handleFirstConnection(int clientSocket);
	void		removeClient(int fd);
	std::string	recvAllData(int clientSocket);

	int		checkNameValidity( std::string &name );
	// void	handleRequestError( int error, Client &user ) const;
	void	handleReplies( int code, std::string param, Channel *chan, Client &client );
	bool	isClientAdded(int fd) const;
	bool	is_good_infos(std::string message, int clientSocket);
	bool	channelExist(std::string channelName);
	bool	nicknameExist(std::string nickname);
	Channel	*findChanByName( std::string channelName );
	Client	*findUserByNick( std::string nick );


	// Commands
	void	createChannel( std::string channelName, Client &client );
	bool	joinErrors(Channel *channel, Client &client);
	void	joinChannel( Channel *channel, Client &client );
	void	ft_join(std::string message, Client &client);

	void	executeModeChannels(Client &client, std::vector<std::string> &params);
	void	executeModeUsers(Client &client, std::vector<std::string> &params);
	Channel *getChannelByName(std::string channelName);

	void	notifyChannel( std::string channelName, std::string &msg, Client &sender );
	void	sendPrivMsg( std::string targetNick, std::string &command, Client &sender );
	void	ft_privMsg( std::string message, Client &client );

	void	ft_kick( std::string command, Client &sender );

	void	ft_topic( std::string &msg, Client &sender );

	void	partCommand( std::string channelName, Client &client, std::string message);
	bool	checkNickValidity( std::string &nick );
	void	ft_nick( std::string &message, Client &client );
	void	ft_user( std::string &message, Client &sender );
	void	quitCommand(Client &client, std::string message);
	void	modeCommand(Client &client, std::string message);
};

