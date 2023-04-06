#pragma once
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include <fcntl.h>
#include <cstdlib>
#include "client.hpp"
#include "channel.hpp"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 512 // Taille du buffer de réception

class Serveur
{
private:
    int                 _serverSocket;
    int			        _port;
    struct sockaddr_in  _serverAddr;
    std::string         _password;
	std::vector<pollfd> _pollFds;
	std::vector <Client *> _clients;
	std::vector <Channel *> _channels;
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
	Client 	*getClientbyFd(int fd);
	int		checkNameValidity( std::string &name )
};

