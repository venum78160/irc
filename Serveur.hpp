#pragma once
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include <map>
#include "client.hpp"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

class Serveur
{
private:
    int                 	_serverSocket;
    int			        	_port;
    struct sockaddr_in  	_serverAddr;
    std::string         	_password;
	std::vector<pollfd> 	_pollFds;
	std::map<int , Client>	_MClient;
public:
    Serveur(std::string password, char *port);
    ~Serveur();
    void	start();
	void	run();
	void	eventClient(Client *Client);
	void	removeClient(int fd);
};

Serveur::Serveur(std::string password, char *port) : _password(password)
{
	try
	{
        _port = std::atoi(port);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Erreur : la chaîne n'est pas un entier valide." << std::endl;
		exit(1);
    } catch (const std::out_of_range& e) {
        std::cerr << "Erreur : la chaîne représente un entier en dehors de la plage de valeurs valides." << std::endl;
		exit(1);
    }
}

Serveur::~Serveur()
{
}

void Serveur::removeClient(int fd) {
    std::vector<pollfd>::iterator it = _pollFds.begin();
    while (it != _pollFds.end()) {
        if (it->fd == fd) {
            close(fd); // fermer le descripteur de fichier
            _pollFds.erase(it); // supprimer le pollfd de la liste
            _MClient.erase(fd); // supprimer le client de la map
            break;
        }
        ++it;
    }
}


void	Serveur::eventClient(Client *Client)
{
	char	buffer[BUFFER_SIZE];
	int bytes_read = recv(Client->GetSocketFD(), Client->GetBuffer(), sizeof(BUFFER_SIZE), 0);
	if (bytes_read <= 0)
	{
        // Le client s'est déconnecté, supprimer de partout
		removeClient(Client->GetSocketFD());
	}
}

void	Serveur::run()
{
	while (true)
	{
        // Attente d'un événement sur l'un des descripteurs de fichiers surveillés
        if (poll(&_pollFds[0], _pollFds.size(), -1) < 0)
		{
            std::cerr << "Erreur lors de l'appel à la fonction poll()" << std::endl;
            break;
        }

        // Parcours de la structure pollfd pour traiter les événements
        for (int i = 0; i < _pollFds.size(); i++)
        {
            // Vérification si un événement s'est produit sur le socket d'écoute
            if (_pollFds[i].fd == _serverSocket && _pollFds[i].revents & POLLIN)
            {

                // Acceptation de la connexion entrante
                struct sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                int clientSocket = accept(_serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
                if (clientSocket < 0) {
                    std::cerr << "Erreur lors de l'acceptation de la connexion entrante" << std::endl;
                    break;
                }

                // Ajout du descripteur de fichier du client à la structure pollfd
                pollfd clientPollFd = {clientSocket, POLLIN, 0};
                _pollFds.push_back(clientPollFd);
				// Ajout du client avec comme pair son fd à la map.
				_MClient.insert(std::make_pair(clientSocket, Client(clientSocket, "val", "venum", "servername", "here")));

                std::cout << "Nouvelle connexion entrante depuis " << inet_ntoa(clientAddr.sin_addr) << std::endl;
            }

            // Vérification si un événement s'est produit sur l'un des sockets des clients
            else if (_pollFds[i].fd != _serverSocket && _pollFds[i].revents & POLLIN)
            {
				int fd_client = _pollFds[i].fd;
				// Client* client = &_MClient[fd_client];
				eventClient(&_MClient[fd_client]);
            }
        }
	}
}

void	Serveur::start()
{
	// Création du socket d'écoute
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverSocket == -1) {
        std::cerr << "Erreur lors de la création du socket d'écoute" << std::endl;
        exit (1);
    }

	// Définition des paramètres du socket d'écoute
	_serverAddr.sin_family = AF_INET;
    _serverAddr.sin_addr.s_addr = INADDR_ANY;
    _serverAddr.sin_port = htons(_port);

	// Association du socket d'écoute à l'adresse et au port spécifiés
    if (bind(_serverSocket, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) < 0) {
        std::cerr << "Erreur lors de l'association du socket d'écoute à l'adresse et au port spécifiés" << std::endl;
        exit (1);
    }

    // Mise en attente de connexions entrantes
    if (listen(_serverSocket, MAX_CLIENTS) < 0) {
        std::cerr << "Erreur lors de la mise en attente de connexions entrantes" << std::endl;
        exit (1);
    }

	// Création de la structure pollfd pour surveiller les descripteurs de fichiers
    pollfd serverPollFd = {_serverSocket, POLLIN, 0};
    _pollFds.push_back(serverPollFd);
	this->run();
	close(_serverSocket);
}
