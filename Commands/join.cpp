/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 18:28:57 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/18 17:31:53 by itaouil          ###   ########.fr       */
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

void	Server::createChannel(std::string channelName, Client &client)
{
	
	Channel *newChannel = new Channel(channelName, client);
	newChannel->addUser(client);
	_channels.push_back(newChannel);
	std::cout << "Channel successfully created: " << channelName << std::endl; // test only, to delete later
	joinChannel(newChannel, client);
}

bool	Server::joinErrors(Channel *channel, Client &client)
{
	if (channel == nullptr)
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
	std::vector<std::string> blacklist = channel->getBlacklist();
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = blacklist.end();
	for (it = blacklist.begin(); it != ite; it++)
	{
		if (!(*it).compare(clientNick))
		{
			handleReplies(ERR_BANNEDFROMCHAN, channel->getName(), nullptr, client);
			return (true);
		}
	}
	// check if channel is full
	if (channel->getUserLimit() > -1 && channel->getNbUsers() >= channel->getUserLimit())
	{
		handleReplies(ERR_CHANNELISFULL, channel->getName(), nullptr, client);
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

	/* reply sucessfully joined :
	- send client list of commands available
	- send client topic of channel with RPL_TOPIC
	- send client list of users in channel with RPL_NAMREPLY */
	client.SetServername(channel->getName());
	client.AddChannel(channel->getName());
	std::cout << "Channel successfully joined: " << channel->getName() << std::endl; // test only, to delete later

	// std::string reply = ":127.0.0.1 " + client.GetNickname() + " JOIN " + channelName + + "\r\n";
	// send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
	handleReplies(RPL_NAMREPLY, "", channel, client);
	handleReplies(RPL_TOPIC, "", channel, client);
}

void	Server::ft_join(std::string message, Client &client)
{
	std::transform(message.begin(), message.end(), message.begin(), ::tolower);

	std::cout << "join message: " << message << std::endl; // test only, to delete later
	std::vector<std::string> params = splitStr(message, ' ');
	if (params.size() < 2) // check if there is enough params
	{
		std::cout << "not enough params" << std::endl; // test only, to delete later
		handleReplies(ERR_NEEDMOREPARAMS, "join", nullptr, client);
		return ;
	}

	std::vector<std::string> channels = splitStr(params[1], ','); // split channels in case there is more than one
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = channels.end();

	for (it = channels.begin(); it != ite; it++) // join each channel
	{
		std::string channelName = *it;
		std::cout << "testing channel: " << channelName << std::endl; // test only, to delete later
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
		{
			std::cout << "creating channel: " << channelName << std::endl; // test only, to delete later
			createChannel(channelName, client);
		}
	}
}