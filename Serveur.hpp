#pragma once
#include "ft_irc.hpp"

class Serveur
{
private:
    int                 _serverSocket;
    int			        _port;
    struct sockaddr_in  _serverAddr;
    std::string         _password;
	std::vector<pollfd> _pollFds;
	std::map <int, Client *> _clients;
	std::vector <Channel *> _channels;
	std::map<int , Client>	_MClient;
public:
    Serveur(std::string password, char *port);
    ~Serveur();
    void	start();
	void	run();
	void	eventClient( pollfd Client);
	void	getServerChannels();
	void	handleMessage(std::string message, int fd);
	void	handleFirstConnection(int clientSocket);
	void	joinCommand(std::string channelName, Client &client);
	void	eventClient(Client *Client);
	void	removeClient(int fd);
};

