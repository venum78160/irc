/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:09:06 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/16 17:28:42 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// /create nom_channel
// Créé un nouveau channel avec pour nom nom_channel,

// /join nom_channel
// Rejoint le channel nom_channel,

// /rename nom_channel nouveau_nom
// Renomme le channel nom_channel en nouveau_nom,

// /part nom_channel
// Quitte le channel nom_channel,

// /delete nom_channel
// Supprime le channel nom_channel,

// /nick nouveau_nom
// Renomme l'utilisateur en nouveau_nom,

// /users
// Affiche la liste des utilisateurs connectés au channel / serveur,

// /list [string]
// Affiche la liste des channels disponibles. Si string est donné en paramètre, affiche la liste des channels contenant string dans leur nom,

// /msg nom_utilisateur message
// Envoie message à nom_utilisateur,
// message
// Envoie un message dans le channel courant.
// kick un utilisateur
// BAn un utilisateur

#include "../ft_irc.hpp"

std::vector<std::string> splitStr( std::string str, char sep )
{
	size_t					  begin;
	size_t					  end = 0;
	std::vector<std::string>	ret;

	begin = str.find_first_not_of(sep, end);
	while (begin != std::string::npos)
	{
		end = str.find(sep, begin);
		std::string newStr = str.substr(begin, end - begin);
		ret.push_back(newStr);
		begin = str.find_first_not_of(sep, end);
	}
	return (ret);
}

// void Server::handleRequestError( int error, Client &user ) const
// {
// 	std::string reply;

// 	if (error == NOTENOUGHPARAMS)
// 		reply = ":127.0.0.1 461 " + user.GetNickname() + " :Not enough parameters\r\n";
// 	else if (error == NAMETOOLONG)
// 		reply = ":127.0.0.1 479 " + user.GetNickname() + " :Channel name too long\r\n";
// 	else if (error == WRONGNAME)
// 		reply = ":127.0.0.1 479 " + user.GetNickname() + " :Channel name contains illegal characters\r\n";

// 	send(user.GetSocketFD(), reply.c_str(), reply.size(), 0);
// }


void	Server::handleMessage(std::string message, Client &client)
{
	std::cout << "Message : " << message << " from " << client << std::endl;
	if (message.find("JOIN") != std::string::npos && message.find("JOIN") == 0)
	{
		std::cout << "in join" << std::endl;
		std::string channelName = message.substr(message.find("JOIN") + 5, message.size());
		this->joinCommand(channelName, client);
	}
}



void	Server::privMsgCommand( std::string command, Client &sender )
{
	Client						target;
	std::string 				targetNick;
	std::string					message;
	std::vector<std::string> 	args = splitStr(command, ' ');

	if (command.find("PRIVMSG") == std::string::npos || command.size() < 9) // function protection, although it should never be verified
		return ;
	if (args.size() < 3)
	{
		// send error message about wrong input
		return ;
	}
	targetNick = args[1]; // retrieving target's nickname from the command
	
	std::map<int, Client>::iterator it;
	std::map<int, Client>::iterator ite = _MClient.end(); // retrieving target and making sure it exists
	for (it = _MClient.begin(); it != ite; *it++)
		if ((it->second).GetNickname() == targetNick)
		{
			target = it->second;
			break ;
		}
	if (it == ite) // in case target doesn't exist
	{
		// send error message about non-existent user
		return ;
	}
	message = sender.getFullId() + " " + command;
	send(target.GetSocketFD(), &message, sizeof(message), 0);
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


void Server::quitCommand(Client &client, std::string message)
{
    std::string quitMessage = message.substr(5);
    if (quitMessage.size() == 0)
        quitMessage = "Client Quit";
    std::string reply = ": " + client.GetNickname() + " QUIT :" + quitMessage + "\r\n";
    send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
    for (size_t i = 0; i < client.GetChannels().size(); i++)
    {
        for (size_t j = 0; j < _channels.size(); j++)
        {
            if (_channels[j]->getName() == client.GetChannels()[i])
            {
                _channels[j]->removeUser(client);
                break ;
            }
        }
    }
    this->removeClient(client.GetSocketFD());
}