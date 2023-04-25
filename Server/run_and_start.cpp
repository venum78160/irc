
#include "../ft_irc.hpp"

void	Server::run()
{
	fcntl(_serverSocket, F_SETFL, O_NONBLOCK); // Set the socket to non-blocking
	while (true)
	{
		// Attente d'un événement sur l'un des descripteurs de fichiers surveillés
		int pollResult = poll(_pollFds.data(), _pollFds.size(), -1);
		if (pollResult < 0)
		{
			std::cerr << "Erreur lors de l'appel à la fonction poll()" << std::endl;
			break;
		}
        if (pollResult == 0)
        {
            std::cerr << "Timeout while waiting for events" << std::endl;
            break;
        }
		// std::cout << "-------je suis apres dans le poll" << std::endl;
        // Parcours de la liste des descripteurs de fichiers surveillés
        for (size_t i = 0; i < _pollFds.size(); i++)
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
				pollfd clientPollFd = {clientSocket, POLLIN | POLLHUP | POLLERR, 0};
				std::cout << "Nouvelle connexion entrante depuis " << inet_ntoa(clientAddr.sin_addr) << std::endl;
				handleFirstConnection(clientSocket);
				if(isClientAdded(clientSocket) == true)
				{
					std::cout << "Client " << _MClient[clientSocket].GetSocketFD() << " " << _MClient[clientSocket].GetNickname() << " ajouté." << std::endl;
				}
                _pollFds.push_back(clientPollFd);
				continue;
            }
			// Vérification si un événement s'est produit sur l'un des sockets des clients
			else if (_pollFds[i].fd != _serverSocket && _pollFds[i].revents & POLLIN)
			{
				int fd_client = _pollFds[i].fd;
				if(isClientAdded(fd_client) == false)
				{
					handleFirstConnection(fd_client);
					if(isClientAdded(fd_client) == false)
					{
						std::cout << "Client " << fd_client << " destruct" << std::endl;
						removeClient(fd_client);
						continue;
					}
					std::cout << "Client " << _MClient[fd_client].GetSocketFD() << " " << _MClient[fd_client].GetNickname() << " ajouté 2." << std::endl;
					continue;
				}
				eventClient(&_MClient[fd_client]);
			}
		}
	}
}

void	Server::start()
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
	std::cout << bg_black red << "Waiting for connections..." << reset << std::endl;
	pollfd serverPollFd = {_serverSocket, POLLIN, 0};
	_pollFds.push_back(serverPollFd);
	this->run();
	close(_serverSocket);
}

// /connect server port password