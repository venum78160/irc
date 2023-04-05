#include "serveur.hpp"

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

void	Serveur::eventClient(pollfd Client)
{
    char	buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_read = recv(Client.fd, buffer, sizeof(buffer), 0);
    if (bytes_read <= 0)
    {
        // Le client s'est déconnecté, supprimer son descripteur de fichier
        std::cout << "Client disconnected" << std::endl;
        //close(Client.fd); on le close déjà dans le POLLHUP event
    }
    else
    {
        // Traiter le message reçu
        std::string message(buffer);
        std::cout << "Message reçu : " << message << std::endl;
        if (Client.revents & POLLOUT)
        {
            std::cout << "Sending message to client" << std::endl;
            send(Client.fd, "We have received ur request", strlen("We have received ur request"), 0);
        }

    }
}

void	Serveur::run()
{
    fcntl(_serverSocket, F_SETFL, O_NONBLOCK); // Set the socket to non-blocking
    while (true)
    {
        // Attente d'un événement sur l'un des descripteurs de fichiers surveillés
        if (poll(&_pollFds[0], _pollFds.size(), -1) < 0)
        {
            std::cerr << "Erreur lors de l'appel à la fonction poll()" << std::endl;
            break;
        }

        // Parcours de la structure pollfd pour traiter les événements
        for (int i = 0; static_cast<size_t>(i) < _pollFds.size(); i++)
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
                pollfd clientPollFd = {clientSocket, POLLIN | POLLOUT | POLLHUP, 0};
                _pollFds.push_back(clientPollFd);

                std::cout << "Nouvelle connexion entrante depuis " << inet_ntoa(clientAddr.sin_addr) << std::endl;
            }
                // Vérification si un événement s'est produit sur l'un des sockets des clients
            else if (_pollFds[i].fd != _serverSocket && _pollFds[i].revents & POLLIN)
                eventClient(_pollFds[i]);
            if (_pollFds[i].fd != _serverSocket && _pollFds[i].revents & POLLHUP)
            {
                //delete client from vector of clients
                close(_pollFds[i].fd);
                _pollFds.erase(_pollFds.begin() + i);
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
        exit(1);
    }

    // Définition des paramètres du socket d'écoute
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_addr.s_addr = INADDR_ANY;
    _serverAddr.sin_port = htons(_port);

    // Association du socket d'écoute à l'adresse et au port spécifiés
    if (bind(_serverSocket, (struct sockaddr *)&_serverAddr, sizeof(_serverAddr)) < 0) {
        std::cerr << "Erreur lors de l'association du socket d'écoute à l'adresse et au port spécifiés" << std::endl;
        exit(1);
    }

    // Mise en attente de connexions entrantes
    if (listen(_serverSocket, MAX_CLIENTS) < 0) {
        std::cerr << "Erreur lors de la mise en attente de connexions entrantes" << std::endl;
        exit(1);
    }

    // Création de la structure pollfd pour surveiller les descripteurs de fichiers
    pollfd serverPollFd = {_serverSocket, POLLIN, 0};
    _pollFds.push_back(serverPollFd);
    this->run();
    close(_serverSocket);
}