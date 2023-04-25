
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

Channel	*Server::findChanByName( std::string channelName )
{
    Channel *chan = NULL;

    std::vector<Channel *>::iterator it;
    std::vector<Channel *>::iterator ite = _channels.end();

    for (it = _channels.begin(); it!= ite; it++)
    {
        if ((*it)->getName() == channelName)
        {
            chan = *it;
            break ;
        }
    }
    return (chan);
}

Client	*Server::findUserByNick( std::string nick )
{
    Client  *user = NULL;
    std::map<int, Client>::iterator it;
    std::map<int, Client>::iterator ite = _MClient.end();

    for (it = _MClient.begin(); it!= ite; it++)
    {
        if (it->second.GetNickname() == nick)
        {
            user = &(it->second);
            break ;
        }
    }
    return (user);
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
			if (isClientAdded(fd) == true)
            	_MClient.erase(fd); // supprimer le client de la map
            break;
        }
        ++it;
    }
}

void	Server::eventClient(Client *client)
{
	std::string message = recvAllData(client->GetSocketFD());
	if (message == "DISCONNECTED")
		return ;
	// std::cout << "[event client] Message reçu : |" << message << "|" <<std::endl;
	// Remplir le vecteur buffer_ de la classe client avec le contenu de message
	// client->printclientInfo();
	std::string newBuffer = client->GetBuffer() + message;
	client->SetBuffer(newBuffer);
	std::string buffer = client->GetBuffer();
	if (buffer.find('\n') == std::string::npos)
		return ;
	this->handleMessage(buffer, *client);
	client->SetBuffer("");
	return ;
}

bool Server::isClientAdded(int fd) const
{
	return _MClient.find(fd) != _MClient.end();
}

int Server::checkNameValidity( std::string &name )
{
	int	nameLen = name.size();
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


// /connect server port password