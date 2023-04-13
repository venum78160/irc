
#include "../ft_irc.hpp"

//******************************//
//   CONSTRUCTOR / DESTRUCTOR	//
//******************************//
Server::Server(std::string password, int port) : _port(port), _password(password) {
}

Server::~Server() {}

//******************************//
// 		  G E T T E R S		    //
//******************************//
std::vector<Channel *> Server::getServerChannels()
{
    return (_channels);
}

//******************************//
// 		  S E T T E R S		    //
//******************************//

//******************************//
// 		  M E T H O D S 	    //
//******************************//

//******************************//
// 		  O P E R A T O R S     //
//******************************//

void Server::removeClient(int fd)
{
	std::vector<pollfd>::iterator it = _pollFds.begin();
	std::vector<pollfd>::iterator ite = _pollFds.end();

    while (it != ite)
    {
        if (it->fd == fd)
        {
			std::cout << "[removeClient] client : " << fd << " is disconnected" << std::endl;
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
	char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
	int bytes_read =recv(Client->GetSocketFD(), buffer, sizeof(buffer), 0);
	if (buffer[bytes_read - 1] == '\n')
        buffer[bytes_read - 1] = '\0';
	std::cout << "bytes read = " << bytes_read<<" [event Client] Message reçu : " << buffer << "|" <<std::endl;
	std::string message(buffer);
	std::cout << "[event Client] Message reçu : |" << message << "|" <<std::endl;
	// Remplir le vecteur buffer_ de la classe Client avec le contenu de message
	Client->SetBuffer(message);
	this->handleMessage(message, *Client);
	return ;
}

bool Server::isClientAdded(int fd) const
{
    return _MClient.find(fd) != _MClient.end();
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
	std::cout << "Message : " << message << " from " << client << std::endl;
	if (message.find("JOIN") != std::string::npos)
	{
		std::cout << "in join" << std::endl;
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
    for (it = _channels.begin(); it != _channels.end(); ++it) {
        if ((*it)->getName() == channelName)
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

// /connect server port password