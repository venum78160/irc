/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 18:28:57 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/16 18:34:15 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_irc.hpp"


// void Server::joinCommand(std::string channelName, Client &client)
// {
// 	int	token = checkNameValidity(channelName);
// 	if (token == NOTENOUGHPARAMS || token == NAMETOOLONG || token == WRONGNAME)
// 	{
// 		handleReplies(token, client);
// 		return ;
// 	}
// 	if (channelName == "0")
// 	{
// 		// JOIN 0 means leave all channels
// 		for (size_t i = 0; i < client.GetChannels().size(); i++)
// 		{
// 			client.RemoveChannel(client.GetChannels()[i]);
// 			for (size_t j = 0; j < _channels.size(); j++)
// 			{
// 				if (_channels[j]->getName() == client.GetChannels()[i])
// 				{
// 					_channels[j]->removeUser(client);
// 					break ;
// 				}
// 			}
// 		}
// 		std::string reply = ": " + client.GetNickname() + " JOIN 0\r\n";
// 		send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
// 		return ;
// 	}
// 	//check if channel already exist
// 	for (size_t i = 0; i < _channels.size(); i++)
// 	{
// 		// channel already exists
// 		if (_channels[i]->getName() == channelName)
// 		{
// 			Channel *newChannel = _channels[i];
// 			try
// 			{
// 				newChannel->addUser(client);
// 			}
// 			catch (const std::exception &e)
// 			{
// 				std::cerr << client.GetNickname() << " : " << e.what() << std::endl;
// 				// make reply for full channels
// 				std::string reply = ":127.0.0.1 471 " + client.GetNickname() + " :Cannot join channel (+l)\r\n";
// 				return;
// 			}
// 			// reply sucessfully joined
// 			std::string reply = ":127.0.0.1 " + client.GetNickname() + " JOIN " + channelName + + "\r\n";
// 			send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
// 			client.SetServername(channelName);
// 			return ;
// 		}
// 	}
// 	// channel doesn't exist
// 	Channel *newChannel = new Channel(channelName, client);
// 	newChannel->addUser(client); // pas besoin de try le channel vient d'être créé il ne peut pas être full
// 	  _channels.push_back(newChannel);
// 	// reply sucessfully joined
// 	std::string reply = ": " + client.GetNickname() + " JOIN " + channelName + "\r\n";
// 	client.SetServername(channelName);
// 	send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
// }

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

void	Server::createChannel(std::string channelName, Client &client)
{
	
	Channel *newChannel = new Channel(channelName, client);
	newChannel->addUser(client);
	_channels.push_back(newChannel);
	joinChannel(newChannel, client);
}

bool	Server::joinErrors(Channel *channel, Client &client)
{
	if (channel == nullptr)
	{
		// JOIN 0 means leave all channels
		// remove user from all channels
		return (true);
	}
	// check if user is banned
	std::string	clientNick = client.GetNickname();
	std::vector<std::string> blacklist = channel->getBlacklist();
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = blacklist.end();
	for (it = blacklist.begin(); it != ite; it++)
	{
		if (!(*it).compare(clientNick))
		{
			handleReplies(ERR_BANNEDFROMCHAN, channel->getName(), client);
			return (true);
		}
	}
	// check if channel is full
	if (channel->getNbUsers() >= channel->getUserLimit())
	{
		handleReplies(ERR_CHANNELISFULL, channel->getName(), client);
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

	try
	{
		channel->addUser(client);
	}
	catch (const std::exception &e) // Channel is full - should never happen because of joinErrors
	{
		handleReplies(ERR_CHANNELISFULL, channel->getName(), client);
		return;
	}
	// reply sucessfully joined
	// send client list of commands available
	// send client topic of channel with RPL_TOPIC
	// send client list of users in channel with RPL_NAMREPLY
	std::string reply = ":127.0.0.1 " + client.GetNickname() + " JOIN " + channelName + + "\r\n";
	send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
	client.SetServername(channelName);
}

void	Server::ft_join(std::string message, Client &client)
{
	std::transform(message.begin(), message.end(), message.begin(), ::tolower);

	std::vector<std::string> params = splitStr(message, ' ');
	if (params.size() < 2) // check if there is enough params
	{
		handleReplies(ERR_NEEDMOREPARAMS, "join", client);
		return ;
	}

	std::vector<std::string> channels = splitStr(params[1], ','); // split channels in case there is more than one
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = channels.end();

	for (it = channels.begin(); it != ite; it++) // join each channel
	{
		std::string channelName = *it;
		if (!channelName.compare("0"))
		{
			joinChannel(nullptr, client);
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
			createChannel(channelName, client);
	}
}