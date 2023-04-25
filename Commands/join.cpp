/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 18:28:57 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/25 21:49:56 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_irc.hpp"

void	Server::createChannel(std::string channelName, Client &client)
{
	Channel *newChannel = new Channel(channelName, client);
	newChannel->addUser(client);
	_channels.push_back(newChannel);
	// std::cout << "Channel successfully created: " << channelName << std::endl; // test only, to delete later
	joinChannel(newChannel, client);
	// std::cout << "giving op rights" << std::endl;
	newChannel->giveOpRights(client);
}

bool	Server::joinErrors(Channel *channel, Client &client)
{
	if (channel == NULL)
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
		return (true);
	}
	// check if user is banned
	std::string	clientNick = client.GetNickname();
	if (channel->isInBlacklist(clientNick))
	{
			handleReplies(ERR_BANNEDFROMCHAN, channel->getName(), NULL, client);
			return (true);
	}
	// check if channel is full
	if (channel->getUserLimit() > -1 && channel->getNbUsers() >= channel->getUserLimit())
	{
		handleReplies(ERR_CHANNELISFULL, channel->getName(), NULL, client);
		return (true);
	}
	return (false);
}

void	Server::joinChannel(Channel *channel, Client &client)
{
	// Things to check :
	// channel name = "0" ? if yes leave all channels
	// is user banned ?
	// is channel full ?

	if (joinErrors(channel, client))
		return ;

	channel->addUser(client);
	client.SetServername(channel->getName());
	client.AddChannel(channel->getName());
	// std::cout << "Channel successfully joined: " << channel->getName() << std::endl; // test only, to delete later
	// send replies
	std::string reply = ":" + client.getFullId() + " JOIN " + channel->getName() + "\r\n";
	channel->broadcastMessageToAll(reply);
	handleReplies(RPL_TOPIC, "", channel, client);
	handleReplies(RPL_NAMREPLY, "", channel, client);
	handleReplies(RPL_ENDOFNAMES, "", channel, client);
}

void	Server::ft_join(std::string message, Client &client)
{
	std::transform(message.begin(), message.end(), message.begin(), ::tolower);

	std::vector<std::string> params = splitStr(message, ' ');
	if (params.size() < 2) // check if there is enough params
	{
		handleReplies(ERR_NEEDMOREPARAMS, "join", NULL, client);
		return ;
	}

	std::vector<std::string> channels = splitStr(params[1], ','); // split channels in case there is more than one
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = channels.end();

	for (it = channels.begin(); it != ite; it++) // join each channel
	{
		std::string channelName = *it;
		if (channelName[channelName.size() - 1] == '\n')
			channelName.erase(channelName.size() - 1, std::string::npos);
		if (channelName[channelName.size() - 1] == '\r')
			channelName.erase(channelName.size() - 1, std::string::npos);
		if (!channelName.compare("0"))
		{
			joinChannel(NULL, client);
			return ;
		}
		// check if channel exists
		std::vector<Channel *>::iterator it2;
		std::vector<Channel *>::iterator ite2 = _channels.end();
		for (it2 = _channels.begin(); it2 != ite2; it2++)
		{
			if (!(*it2)->getName().compare(channelName)) // channel exists so we join it
			{
				joinChannel(*it2, client);
				break ;
			}
		}
		if (it2 == ite2) // channel doesn't exist so we create it
		{
			createChannel(channelName, client);
		}
	}
}
