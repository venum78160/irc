
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
				handleFirstConnection(clientSocket);
				std::cout << "Nouvelle connexion entrante depuis " << inet_ntoa(clientAddr.sin_addr) << std::endl;
				if(isClientAdded(clientSocket) == true)
				{
					std::cout << "Client " << _MClient[clientSocket].GetNickname() << " ajouté." << std::endl;
					_pollFds.push_back(clientPollFd);
				}
				continue;
            }
			// Vérification si un événement s'est produit sur l'un des sockets des clients
			else if (_pollFds[i].fd != _serverSocket && _pollFds[i].revents & POLLIN)
			{
                // check if fd exist in _MClient
                // if (_MClient.find(_pollFds[i].fd) == _MClient.end())
                // {
                //     handleFirstConnection(_pollFds[i].fd);
                //     continue;
                // }
				int fd_client = _pollFds[i].fd;
				eventClient(&_MClient[fd_client]);
                if (_pollFds[i].revents & (POLLERR | POLLHUP))
                {
                    std::cerr << "Client disconnected 2" << std::endl;
                    removeClient(_pollFds[i].fd);
                }
			}
			else if (_pollFds[i].revents & (POLLHUP | POLLERR))
			{
				std::cout << "Déconnexion du client" << std::endl;
				removeClient(_pollFds[i].fd);
			}
		}
	}
}

// /connect server port password