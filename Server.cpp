
#include "ft_irc.hpp"

Server::Server(std::string password, int port) : _port(port), _password(password) {
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
			std::cout << "[event Client] client : " << fd << " is disconnected" << std::endl;
            close(fd); // fermer le descripteur de fichier
            _pollFds.erase(it); // supprimer le pollfd de la liste
            _MClient.erase(fd); // supprimer le client de la map
            break;
        }
        ++it;
    }
}

std::vector<Channel *> Server::getServerChannels()
{
    return (_channels);
}

void	Server::eventClient(Client *Client)
{
	char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
	int bytes_read = recv(Client->GetSocketFD(), buffer, sizeof(buffer), 0);
    if (bytes_read == -1)
    {
        //std::cerr << "Error while receiving data from client by eventClient" << std::endl;
        removeClient(Client->GetSocketFD());
        return;
    }
    // handle client disconnection
    else if (bytes_read == 0)
    {
        std::cerr << "Client disconnected" << std::endl;
        removeClient(Client->GetSocketFD());
        return;
    }
	else
    {
        // Traiter le message reçu
		std::string message(buffer);
        std::cout << "[event Client] Message reçu : " << message << std::endl;
		// Remplir le vecteur buffer_ de la classe Client avec le contenu de message
        Client->SetBuffer(message);
        this->handleMessage(message, *Client);
        return ;
    }
}

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
				_pollFds.push_back(clientPollFd);
				// Ajout du client avec comme pair son fd à la map.
				std::cout << "Nouvelle connexion entrante depuis " << inet_ntoa(clientAddr.sin_addr) << std::endl;
            }
			// Vérification si un événement s'est produit sur l'un des sockets des clients
			else if (_pollFds[i].fd != _serverSocket && _pollFds[i].revents & POLLIN)
			{
                // check if fd exist in _MClient
                if (_MClient.find(_pollFds[i].fd) == _MClient.end())
                {
                    handleFirstConnection(_pollFds[i].fd);
                    continue;
                }
				int fd_client = _pollFds[i].fd;
				eventClient(&_MClient[fd_client]);
                if (_pollFds[i].revents & (POLLERR | POLLHUP))
                {
                    std::cerr << "Client disconnected" << std::endl;
                    removeClient(_pollFds[i].fd);
                }
			}
			else if (_pollFds[i].revents & (POLLHUP | POLLERR))
			{
				std::cout << "Déconnexion du client" << std::endl;
				removeClient(_pollFds[i].fd);
			}
            // send message to client to check if already connected
           /* for (std::map<int, Client>::iterator it = _MClient.begin(); it != _MClient.end(); it++)
            {
                if (it->second.GetSocketFD() != _serverSocket)
                {
                    std::string message = "PING";
                    int bytes_sent = send(it->second.GetSocketFD(), message.c_str(), message.length(), 0);
                    if (bytes_sent < 0)
                    {
                        std::cerr << "Error while sending data to client" << std::endl;
                        removeClient(it->second.GetSocketFD());
                    }
                }
            }*/
		}
	}
}

//write handleFirstConnection to handle the first connection of a new IRC client
int Server::handleFirstConnection(int clientSocket) {
    //recv data give at first connection and answer knowing that the server need a password
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_read = recv(clientSocket, buffer, sizeof(buffer), 0);
    // handle recv error
    if (bytes_read == -1) {
        std::cerr << "Error while receiving data from client" << std::endl;
        removeClient(clientSocket);
        return (0);
    }
    // handle client disconnection
    else if (bytes_read == 0)
    {
        std::cerr << "Client disconnected" << std::endl;
        removeClient(clientSocket);
        return (0);
    }
    std::string message(buffer);
    std::cout << "[fiest co] Message reçu : " << message << std::endl;
    if (message.find("PASS") == std::string::npos) {
        std::string reply = ":127.0.0.1 461 * PASS :No password supplied\r\n";
        send(clientSocket, reply.c_str(), reply.size(), 0);
        return (0);
    }
    if (message.find("NICK") == std::string::npos) {
        std::string reply = ": 461 * NICK :No nickname given\r\n";
        send(clientSocket, reply.c_str(), reply.size(), 0);
        return (0);
    }
    if (message.find("USER") == std::string::npos) {
        std::string reply = ": 461 * USER :Not enough parameters\r\n";
        send(clientSocket, reply.c_str(), reply.size(), 0);
        return (0);
    }
    // if all is good, we can add the client to the map
    std::string::size_type pass_pos = message.find("PASS ");
    std::string::size_type nick_pos = message.find("NICK ");
    std::string::size_type user_pos = message.find("USER ");
    std::string password = message.substr(pass_pos + 5, nick_pos - pass_pos - 6);
    std::string nickname = message.substr(nick_pos + 5, user_pos - nick_pos - 6);
    std::string username = message.substr(user_pos + 5, message.size() - user_pos - 6);
    password.erase(std::remove(password.begin(), password.end(), '\r'), password.end());
    password.erase(std::remove(password.begin(), password.end(), '\n'), password.end());
    nickname.erase(std::remove(nickname.begin(), nickname.end(), '\r'), nickname.end());
    nickname.erase(std::remove(nickname.begin(), nickname.end(), '\n'), nickname.end());
    username.erase(std::remove(username.begin(), username.end(), '\r'), username.end());
    username.erase(std::remove(username.begin(), username.end(), '\n'), username.end());
    username = username.substr(0, username.find(" "));
    if (password == _password)
    {
        for (std::map<int, Client>::iterator it = _MClient.begin(); it != _MClient.end(); ++it)
        {
            if (it->second.GetNickname() == nickname)
            {
                std::string reply = ":127.0.0.1 433 " + nickname + " :Nickname is already in use\r\n";
                send(clientSocket, reply.c_str(), reply.size(), 0);
                return (0);
            }
        }
        //inserer le client dans la map
        _MClient.insert(std::make_pair(clientSocket, Client()));
        _MClient[clientSocket].SetNickname(nickname);
        _MClient[clientSocket].SetUsername(username);
        _MClient[clientSocket].SetServername("");
        _MClient[clientSocket].SetMode("online");
        std::cout << "Client " << nickname << " Username : " << username << " connected" << std::endl;
        std::string reply = ":127.0.0.1 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!" + username + "@HOST\r\n";
        send(clientSocket, reply.c_str(), reply.size(), 0);
        reply = ":127.0.0.1 002 " + nickname + " :Your host is ourIRC, running version 1.0\r\n";
        send(clientSocket, reply.c_str(), reply.size(), 0);
        reply = ":127.0.0.1 003 " + nickname + " :This server was created not a long time ago\r\n";
        send(clientSocket, reply.c_str(), reply.size(), 0);
        reply = ":127.0.0.1 004 ourIRC 1.0";
        send(clientSocket, reply.c_str(), reply.size(), 0);
        reply = ":127.0.0.1 005 " + nickname + " PREFIX=(ov)@+ CHANTYPES=# CHARSET=UTF-8 :are supported by this server\r\n";
        send(clientSocket, reply.c_str(), reply.size(), 0);
        reply = ":127.0.0.1 376 " + nickname + " :End of /MOTD command.\r\n";
        send(clientSocket, reply.c_str(), reply.size(), 0);
        std::cout << "Sending to client" << std::endl;
        return (1);
    }
    else
    {
        std::string reply = ":127.0.0.1 464 " + nickname + " :Password incorrect\r\n";
        send(clientSocket, reply.c_str(), reply.size(), 0);
        return (0);
    }
}
//
//void Server::handleFirstConnection(int clientSocket)
//{
//    //recv NICK and PASS and then reply with good format
//	std::cout << "[handleconnection] :" << clientSocket << "|" << std::endl;
//    char buffer[BUFFER_SIZE];
//    memset(buffer, 0, BUFFER_SIZE);
//	int bytes_read = recv(clientSocket, buffer, sizeof(buffer), 0);
//	// if bytes_read = -1 alors netcat sinon autre irc
//	std::cout << "bytes_read :" << bytes_read << std::endl;
//	_MClient.insert(std::make_pair(clientSocket, Client()));
//	_MClient[clientSocket].SetSock_fd(clientSocket);
//	_MClient[clientSocket].SetNickname("Nickkk");
//	_MClient[clientSocket].SetUsername("Userrr");
//	_MClient[clientSocket].SetServername("");
//	_MClient[clientSocket].SetMode("online");
//    std::string message(buffer);
//    std::cout << "Message reçu : " << message << std::endl;
//}

// void Server::handleFirstConnection(int clientSocket)
// {
//     //recv NICK and PASS and then reply with good format
// 	std::cout << "[handleconnection] :" << clientSocket << "|" << std::endl;
//     char buffer[BUFFER_SIZE];
//     std::string nickname;
//     std::string password;
//     std::string username;
//     memset(buffer, 0, BUFFER_SIZE);
//     // int bytes_read = recv(clientSocket, buffer, sizeof(buffer), 0);
//     // std::cout << "bytes_read : " << bytes_read << std::endl;
// 	recv(clientSocket, buffer, sizeof(buffer), 0);
//     // if (bytes_read <= 0)
//     // {
//     //     // Le client s'est déconnecté, supprimer son descripteur de fichier
//     //     std::cout << "Client disconnected by recv" << std::endl;
//     //     removeClient(clientSocket);
//     // }
//     // else
//     // {
//         // Traiter le message reçu
//         std::string message(buffer);
//         std::cout << "Message reçu : " << message << std::endl;
//         if (message.find("NICK") != std::string::npos && message.find("PASS") != std::string::npos && message.find("USER") != std::string::npos)
//         {
//             std::string::size_type pass_pos = message.find("PASS ");
//             std::string::size_type nick_pos = message.find("NICK ");
//             std::string::size_type user_pos = message.find("USER ");
//             std::string password = message.substr(pass_pos + 5, nick_pos - pass_pos - 6);
//             std::string nickname = message.substr(nick_pos + 5, user_pos - nick_pos - 6);
//             std::string username = message.substr(user_pos + 5, message.size() - user_pos - 6);
//             if (password == _password)
//             {
//                 std::string reply = ":127.0.0.1 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!" + username + "@HOST";
//                 send(clientSocket, reply.c_str(), reply.size(), 0);
//                 for (std::map<int, Client>::iterator it = _MClient.begin(); it != _MClient.end(); ++it)
//                 {
//                     if (it->second.GetNickname() == nickname)
//                     {
//                         std::string reply = ":127.0.0.1 433 " + nickname + " :Nickname is already in use";
//                         send(clientSocket, reply.c_str(), reply.size(), 0);
//                         return ;
//                     }
//                 }
//                 //inserer le client dans la map
//                 _MClient.insert(std::make_pair(clientSocket, Client()));
//                 _MClient[clientSocket].SetNickname(nickname);
//                 _MClient[clientSocket].SetUsername(username);
//                 _MClient[clientSocket].SetServername("");
//                 _MClient[clientSocket].SetMode("online");
//             }
//             else
//             {
//                 std::string reply = ":127.0.0.1 464 " + nickname + " :Password incorrect";
//                 send(clientSocket, reply.c_str(), reply.size(), 0);
//             }
//         }
//         else
//         {
//             std::string reply = ":127.0.0.1 464 " + nickname + " :Password incorrect";
//             send(clientSocket, reply.c_str(), reply.size(), 0);
//         }
//     // }
// }

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

void Server::handleRequestError( int error, Client &user ) const
{
	std::string reply;

	if (error == NOTENOUGHPARAMS)
		reply = ":127.0.0.1 461 " + user.GetNickname() + " :Not enough parameters\r\n";
	else if (error == NAMETOOLONG)
		reply = ":127.0.0.1 479 " + user.GetNickname() + " :Channel name too long\r\n";
	else if (error == WRONGNAME)
		reply = ":127.0.0.1 479 " + user.GetNickname() + " :Channel name contains illegal characters\r\n";

	send(user.GetSocketFD(), reply.c_str(), reply.size(), 0);
}

int Server::checkNameValidity( std::string &name )
{
	int	nameLen = name.size();
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i]->getName() == name)
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

void	Server::handleMessage(std::string message, Client &client)
{
	std::cout << "Message : " << message << std::endl;
	std::cout << "Client : " << client << std::endl;
	if (message.find("JOIN") != std::string::npos)
	{
		std::string channelName = message.substr(message.find("JOIN") + 5, message.size());
		this->joinCommand(channelName, client);
	}
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
			for (size_t j = 0; j < _channels.size(); j++)
			{
				if (_channels[j]->getName() == client.GetChannels()[i])
				{
					_channels[j]->removeUser(client);
					break ;
				}
			}
		}
		std::string reply = ": " + client.GetNickname() + " JOIN 0\r\n";
		send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
		return ;
	}
	//check if channel already exist
	for (size_t i = 0; i < _channels.size(); i++)
	{
		// channel already exists
		if (_channels[i]->getName() == channelName)
		{
			Channel *newChannel = _channels[i];
			try
			{
				newChannel->addUser(client);
			}
			catch (const std::exception &e)
			{
				std::cerr << client.GetNickname() << " : " << e.what() << std::endl;
				// make reply for full channels
				std::string reply = ":127.0.0.1 471 " + client.GetNickname() + " :Cannot join channel (+l)\r\n";
				return;
			}
			// reply sucessfully joined
			std::string reply = ":127.0.0.1 " + client.GetNickname() + " JOIN " + channelName + + "\r\n";
			send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
			client.SetServername(channelName);
			return ;
		}
	}
	// channel doesn't exist
	Channel *newChannel = new Channel(channelName, client);
	newChannel->addUser(client); // pas besoin de try le channel vient d'être créé il ne peut pas être full
	  _channels.push_back(newChannel);
	// reply sucessfully joined
	std::string reply = ": " + client.GetNickname() + " JOIN " + channelName + "\r\n";
	client.SetServername(channelName);
	send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
}

void Server::partCommand(std::string channelName, Client &client)
{
	// Commande : PART #channel ou /part #channel
	// 1 - Mettre en minuscule le nom du channel car il est insensible à la casse
	// 2 - Checker si le channel existe -> si oui, le supprimer de la liste des channels du client, si non, envoyer un message d'erreur
	// 3 - Supprimer le client de la liste des clients du channel

	// 1
	std::transform(channelName.begin(), channelName.end(), channelName.begin(), ::tolower);

	// 2
	Channel *channelToLeave;

    //find channel in _channels by channelName in c++98
    std::vector<Channel*>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); ++it)
    {
        if ((*it)->getName() == channelName) // channel trouvé dans le vector
        {
            channelToLeave = *it;
            channelToLeave->removeUser(client);
            for (size_t i = 0; i < client.GetChannels().size(); i++)
            {
                if (client.GetChannels()[i] == channelName)
                {
                    client.RemoveChannel(channelName);
                    std::string reply = ": " + client.GetNickname() + " PART " + channelName + "\r\n";
                    send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                    return ;
                }
            }
        }
    }
    // not found so send error
}
