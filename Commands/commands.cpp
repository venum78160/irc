/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:09:06 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/24 18:22:26 by itaouil          ###   ########.fr       */
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

void	Server::handleMessage(std::string message, Client &client)
{
	std::cout << "Message : " << message << " from " << client << std::endl;
	if (message.find("JOIN") != std::string::npos && message.find("JOIN") == 0)
	{
		std::cout << "in join" << std::endl;
		// std::string channelName = message.substr(message.find("JOIN") + 5, message.size());
		this->ft_join(message, client);
	}
	if (message.find("PRIVMSG") != std::string::npos && message.find("PRIVMSG") == 0)
	{
		std::cout << "in privmsg" << std::endl;
		std::string privmsg = message.substr(message.find("PRIVMSG") + 8, message.size()); // attention au segfault
		this->ft_privMsg(privmsg, client);
	}
	// if (message.find("MODE") != std::string::npos && message.find("MODE") == 0)
	// {
	//	 std::cout << "in mode" << std::endl;
	//	 this->modeCommand(client, message);
	// }
	if (message.find("PING") != std::string::npos && message.find("PING") == 0)
	{
		std::cout << "in ping" << std::endl;
		std::string reply = "PONG " + message.substr(5);
		send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
	}
	if (message.find("KICK") != std::string::npos && message.find("KICK") == 0)
	{
		std::cout << "in kick" << std::endl;
		if (message.size() < 6) // pour éviter le segfault
		{
			handleReplies(ERR_NEEDMOREPARAMS, "KICK", NULL, client);
			return ;
		}
		std::string kickmsg = message.substr(message.find("KICK") + 5, message.size());
		this->ft_kick(kickmsg, client);
	}
	if (message.find("TOPIC") != std::string::npos && message.find("TOPIC") == 0)
	{
		std::cout << "in topic" << std::endl;
		this->ft_topic(message, client);
	}
	if (message.find("NICK") != std::string::npos && message.find("NICK") == 0)
	{
		std::cout << "in nick" << std::endl;
		this->ft_nick(message, client);
	}
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
	std::cout << "Successfully quitted " << std::endl; // test only, to delete later
}

void Server::modeCommand(Client &client, std::string message)
{
	std::vector<std::string> tab = split(message, ' ');
	// check if format is MODE #channel or nickname modechars modeparams
	if (tab.size() < 3)
		std::string reply = ": " + client.GetNickname() + " 461 " + client.GetNickname() + " :Not enough parameters\r\n";
	else
	{
		// check if first param is a channel or a nickname
		if (tab[1][0] == '#')
		{
			// we are in channel
			if (!channelExist(tab[1]))
			{
				std::string reply = ": " + client.GetNickname() + " 403 " + client.GetNickname() + " :No such channel\r\n";
				send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
				return ;
			}
			executeModeChannels(client, tab);
		}
		else
		{
			// we are in nickname
			if (!nicknameExist(tab[1]))
			{
				std::string reply = ": " + client.GetNickname() + " 401 " + client.GetNickname() + " :No such nick/channel\r\n";
				send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
				return ;
			}
			executeModeUsers(client, tab);
		}
	}
}

