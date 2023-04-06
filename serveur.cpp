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
    //delete all clients if remains
    for (int i = 0; i < _clients.size(); i++)
    {
        delete _clients[i];
    }
    for (int i = 0; i < _channels.size(); i++)
    {
        delete _channels[i];
    }
}

void Serveur::getServerChannels()
{
    return (_channels);
}

void Serveur::handleFirstConnection(int clientSocket)
{
    //recv NICK and PASS and then reply with good format
    char buffer[BUFFER_SIZE];
    std::string nickname;
    std::string password;
    std::string username;
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_read = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytes_read <= 0)
    {
        // Le client s'est déconnecté, supprimer son descripteur de fichier
        std::cout << "Client disconnected by recv" << std::endl;
        close(clientSocket);
    }
    else
    {
        // Traiter le message reçu
        std::string message(buffer);
        std::cout << "Message reçu : " << message << std::endl;
        if (message.find("NICK") != std::string::npos && message.find("PASS") != std::string::npos && message.find("USER") != std::string::npos)
        {
            nickname = message.substr(message.find("NICK") + 5, message.find("PASS") - 1);
            password = message.substr(message.find("PASS") + 5, message.find("USER") - 1);
            username = message.substr(message.find("USER") + 5, message.find("HOST") - 1);
            if (password == _password)
            {
                std::string reply = ":127.0.0.1 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!" + username + "@HOST";
                send(clientSocket, reply.c_str(), reply.size(), 0);
                if (std::find(_clients.begin(), _clients.end(), username) == _clients.end())
                {
                    Client *client = new Client(clientSocket, nickname, username, "", "invisible");
                    _clients.push_back(client);
                }
                else
                {
                    std::string reply = ":127.0.0.1 433 " + nickname + " :Nickname is already in use";
                    send(clientSocket, reply.c_str(), reply.size(), 0);
                }
            }
            else
            {
                std::string reply = ":127.0.0.1 464 " + nickname + " :Password incorrect";
                send(clientSocket, reply.c_str(), reply.size(), 0);
            }
        }
        else
        {
            std::string reply = ":127.0.0.1 464 " + nickname + " :Password incorrect";
            send(clientSocket, reply.c_str(), reply.size(), 0);
        }
    }
}

void Serveur::joinCommand(std::string channelName, Client &client)
{
    if (channelName[0] != '#')
    {
        std::reply = ":127.0.0.1 461 " + client.GetNickname() + " :Not enough parameters";
        send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
    }
    if (channelName == "0")
    {
        // JOIN 0 mean leave all channels
        for (int i = 0; i < client.GetChannels().size(); i++)
        {
            client.RemoveChannel(client.GetChannels()[i]);
        }
        std::string reply = ": " + client.GetNickname() + " JOIN 0";
        return ;
    }
    if (channelName.size() > 50)
    {
        std::string reply = ":127.0.0.1 479 " + client.GetNickname() + " :Channel name too long";
        send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
        return ;
    }
    if (std::find(_channels.begin(), _channels.end(), channelName) == _channels.end())
    {
        Channel *newChannel = new Channel(channelName, client);
        try
            newChannel->addUser(client);
        catch (const std::exception& e)
        {
            std::cerr << client.GetNickname() << " : " << e.what() << std::endl;
            // make reply for full channels
            std::string reply = ":127.0.0.1 471 " + client.getNickname() + " :Cannot join channel (+l)";
            return ;
        }
        _channels.push_back(newChannel);
    }
    else
    {
        for (int i = 0; i < _channels.size(); i++)
        {
            if (_channels[i]->GetName() == channelName)
            {
                newChannel = _channels[i];
                break;
            }
        }
        try
            newChannel->addUser(client);
        catch (const std::exception &e) {
            std::cerr << client.GetNickname() << " : " << e.what() << std::endl;
            // make reply for full channels
            std::string reply = ":127.0.0.1 471 " + client.GetNickname() + " :Cannot join channel (+l)";
            return;
        }
        // reply sucessfully joined
        std::string reply = ":127.0.0.1 " + client.GetNickname() + " JOIN " + channelName;
        client.SetServername(channelName);
    }
}

void    Serveur::handleMessage(std::string message, int fd)
{
    Client *client = getClientbyFd(fd);
    if (message.find("JOIN") != std::string::npos)
    {
        std::string channelName = message.substr(message.find("JOIN") + 5, message.size());
        joinCommand(channelName, *client);
    }
}

void	Serveur::eventClient(pollfd Client)
{
    char	buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_read = recv(Client.fd, buffer, sizeof(buffer), 0);
    if (bytes_read <= 0)
    {
        // Le client s'est déconnecté, supprimer son descripteur de fichier
        std::cout << "Client disconnected by recv" << std::endl;
        close(Client.fd);
    }
    else
    {
        // Traiter le message reçu
        std::string message(buffer);
        std::cout << "Message reçu : " << message << std::endl;
        Serveur::handleMessage(message, Client.fd);
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
                Serveur::handleFirstConnection(clientSocket);
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