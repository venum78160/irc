
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
			if (isClientAdded(fd) == true)
			{
            	_MClient.erase(fd); // supprimer le client de la map
			}
            break;
        }
        ++it;
    }
}

void	Server::eventClient(Client *Client)
{
	std::string message = recvAllData(Client->GetSocketFD());
	std::cout << "[event Client] Message reçu : |" << message << "|" <<std::endl;
	// Remplir le vecteur buffer_ de la classe Client avec le contenu de message
	Client->SetBuffer(message);
	// Client->printClientInfo();
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


// /connect server port password