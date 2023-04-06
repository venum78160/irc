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
	std::map<int , Client>	_MClient;
public:
    Serveur(std::string password, char *port);
    ~Serveur();
    void	start();
	void	run();
	void	eventClient(Client *Client);
	void	removeClient(int fd);
};

