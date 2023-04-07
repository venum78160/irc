
#include "ft_irc.hpp"

Server::Server(std::string password, char *port) : _password(password) {
    try // plutôt try et catch dans le main avant d'envoyer port au constructeur
    {
    _port = std::atoi(port);
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "Erreur : la chaîne n'est pas un entier valide." << std::endl;
		exit(1);
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << "Erreur : la chaîne représente un entier en dehors de la plage de valeurs valides." << std::endl;
		exit(1);
    }
}

Server::~Server() { }

void Server::removeClient(int fd)
{
    std::vector<pollfd>::iterator it = _pollFds.begin();
    std::vector<pollfd>::iterator ite = _pollFds.end();

    while (it != ite)
    {
        if (it->fd == fd)
        {
            close(fd); // fermer le descripteur de fichier
            _pollFds.erase(it); // supprimer le pollfd de la liste
            _MClient.erase(fd); // supprimer le client de la map
            break;
        }
        ++it;
    }
}

void	Server::eventClient(Client *Client)
{
	int bytes_read = recv(Client->GetSocketFD(), Client->GetBuffer(), sizeof(BUFFER_SIZE), 0);
	if (bytes_read <= 0)
	{
        // Le client s'est déconnecté, supprimer de partout
		removeClient(Client->GetSocketFD());
	}
	else
    {
        // Traiter le message reçu
        std::string message(Client->GetBuffer());
        std::cout << "Message reçu : " << message << std::endl;
        this->handleMessage(message, *Client);
    }
}

void	Server::run()
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
                pollfd clientPollFd = {clientSocket, POLLIN, 0};
                _pollFds.push_back(clientPollFd);
				// Ajout du client avec comme pair son fd à la map.
				handleFirstConnection(clientSocket);
				//_MClient.insert(std::make_pair(clientSocket, Client(clientSocket, "val", "venum", "servername", "here"))); // a voir avec ange

                std::cout << "Nouvelle connexion entrante depuis " << inet_ntoa(clientAddr.sin_addr) << std::endl;
            }

            // Vérification si un événement s'est produit sur l'un des sockets des clients
            else if (_pollFds[i].fd != _serverSocket && _pollFds[i].revents & POLLIN)
            {
				int fd_client = _pollFds[i].fd;
				// Client* client = &_MClient[fd_client];
				eventClient(&_MClient[fd_client]);
            }
			if (_pollFds[i].fd != _serverSocket && _pollFds[i].revents & POLLHUP)
            {
                removeClient(_pollFds[i].fd);
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
    pollfd serverPollFd = {_serverSocket, POLLIN, 0};
    _pollFds.push_back(serverPollFd);
	this->run();
	close(_serverSocket);
}



// Server::~Server()
// {
//     //delete all clients map if remains
//     for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
//     {
//         delete it->second;
//     }
//     for (size_t i = 0; i < _channels.size(); i++)
//     {
//         delete _channels[i];
//     }
// }

std::vector<Channel> Server::getServerChannels()
{
    return (_channels);
}

void Server::handleFirstConnection(int clientSocket)
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
        removeClient(clientSocket);
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
                for (std::map<int, Client>::iterator it = _MClient.begin(); it != _MClient.end(); ++it)
                {
                    if (it->second.GetNickname() == nickname)
                    {
                        std::string reply = ":127.0.0.1 433 " + nickname + " :Nickname is already in use";
                        send(clientSocket, reply.c_str(), reply.size(), 0);
                        return ;
                    }
                }
                //inserer le client dans la map
                _MClient.insert(std::make_pair(clientSocket, Client()));
                _MClient[clientSocket].SetNickname(nickname);
                _MClient[clientSocket].SetUsername(username);
                _MClient[clientSocket].SetServername("");
                _MClient[clientSocket].SetMode("online");
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

void Server::handleRequestError( int error, Client &user ) const
{
	std::string reply;

	if (error == NOTENOUGHPARAMS)
		reply = ":127.0.0.1 461 " + user.GetNickname() + " :Not enough parameters";
	else if (error == NAMETOOLONG)
		reply = ":127.0.0.1 479 " + user.GetNickname() + " :Channel name too long";
	else if (error == WRONGNAME)
		reply = ":127.0.0.1 479 " + user.GetNickname() + " :Channel name contains illegal characters";

	send(user.GetSocketFD(), reply.c_str(), reply.size(), 0);
}

int Server::checkNameValidity( std::string &name )
{
	int	nameLen = name.size();
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i].getName() == name)
            return (CHANNELALREADYEXISTS);
    }
	if (nameLen > 50)
		return (NAMETOOLONG);
	if (name[0] != '#')
		return (NOTENOUGHPARAMS);
	for (int i = 0; i < nameLen; i++)
	{
		if (isspace(name[i]) || name[i] == ',' || name[i] == 7)
			return (WRONGNAME);
	}
	return (VALIDNAME);
}

void Server::joinCommand(std::string channelName, Client &client)
{
	std::transform(channelName.begin(), channelName.end(), channelName.begin(), ::tolower);
    int	token = checkNameValidity(channelName);
    if (token == NOTENOUGHPARAMS || token == NAMETOOLONG || token == WRONGNAME)
    {
		handleRequestError(token, client);
		return ;
    }
    if (channelName == "0")
    {
        // JOIN 0 means leave all channels
        for (size_t i = 0; i < client.GetChannels().size(); i++)
        {
            client.RemoveChannel(client.GetChannels()[i]);
			// enlever aussi le client dans la classe channel TODO
        }
        std::string reply = ": " + client.GetNickname() + " JOIN 0";
		send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
        return ;
    }
    //check if channel already exist
    for (size_t i = 0; i < _channels.size(); i++)
    {
        // channel already exists
        if (_channels[i].getName() == channelName)
        {
            Channel newChannel = _channels[i];
            try
            {
                newChannel.addUser(client);
            }
            catch (const std::exception &e)
            {
                std::cerr << client.GetNickname() << " : " << e.what() << std::endl;
                // make reply for full channels
                std::string reply = ":127.0.0.1 471 " + client.GetNickname() + " :Cannot join channel (+l)";
                return;
            }
            // reply sucessfully joined
            std::string reply = ":127.0.0.1 " + client.GetNickname() + " JOIN " + channelName;
            send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
            client.SetServername(channelName);
            return ;
        }
    }
    // channel doesn't exist
    Channel newChannel(channelName, client);
    newChannel.addUser(client); // pas besoin de try le channel vient d'être créé il ne peut pas être full
      _channels.push_back(newChannel);
    // reply sucessfully joined
    std::string reply = ": " + client.GetNickname() + " JOIN " + channelName;
    client.SetServername(channelName);
    send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
}

void    Server::handleMessage(std::string message, Client &client)
{
    if (message.find("JOIN") != std::string::npos)
    {
        std::string channelName = message.substr(message.find("JOIN") + 5, message.size());
        this->joinCommand(channelName, client);
    }
}
