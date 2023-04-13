/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42nice.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:09:06 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/13 17:14:08 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

void    parseCommand(std::string message, Client &client)
{
	
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
    else if 
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
