/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 18:28:57 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/16 16:23:50 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_irc.hpp"

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


void	Server::joinChannel(std::string channelName, Client &client)
{
	// 
}

void	Server::ft_join(std::string message, Client &client)
{
	std::transform(message.begin(), message.end(), message.begin(), ::tolower);

	std::vector<std::string> params = splitStr(message, ' ');
	if (params.size() < 2) // check if there is enough params
	{
		errReplies(ERR_NEEDMOREPARAMS, "join", client);
		return ;
	}

	std::vector<std::string> channels = splitStr(params[1], ','); // split channels in case there is more than one
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = channels.end();

	for (it = channels.begin(); it != ite; it++) // join each channel
	{
		// check if channel name is valid
		std::string channelName = *it;
		joinCommand(channelName, client);
	}
}