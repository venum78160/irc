#pragma once
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include "client.hpp"
#include <fcntl.h>
#include <cstdlib>

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
public:
    Serveur(std::string password, char *port);
    ~Serveur();
    void	start();
	void	run();
	void	eventClient( pollfd Client);
};

