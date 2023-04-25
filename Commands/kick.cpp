/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 01:44:45 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/25 21:34:13 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_irc.hpp"

void	kickReplies( Client &kicker, Client &target, Channel &channel, const char *reason)
{
	std::string		msg;
	std::string		reasonStr;

	if (reason != NULL)
		reasonStr = reason;
	else
		reasonStr = kicker.GetNickname();
	msg = ":" + kicker.GetNickname() + " KICK " + channel.getName() + " " + target.GetNickname() + " :" + reasonStr + "\r\n";
	channel.broadcastMessageToAll(msg);
}
std::vector<std::string> splitThreeArgs( std::string command )
{
	std::vector<std::string>	ret;
	size_t						space = 0;
	size_t						nextSpace = 0;

	while (ret.size() < 3 && nextSpace != std::string::npos)
	{
		space = command.find_first_not_of(" ", nextSpace);
		nextSpace = command.find(" ", space);
		if (ret.size() == 2 || nextSpace == std::string::npos)
			ret.push_back(command.substr(space, std::string::npos));
		else
			ret.push_back(command.substr(space, nextSpace - space));
	}
	return (ret);
}


void	Server::ft_kick( std::string command, Client &sender )
{
	// we need a vector with 2 args min, 3 args max : first one is the channel, second one is the user, third and optional one is the message that can contain spaces

	std::vector<std::string>	params = splitThreeArgs(command);
	if (params.size() < 2) // missing arguments
	{
		handleReplies(ERR_NEEDMOREPARAMS, "KICK", NULL, sender);
		return ;
	}

	Channel		*channel = NULL;
	Client		*target = NULL;
	std::string	senderNick = sender.GetNickname();
	std::string channelName = params[0];
	std::string	targetNick = params[1];
	const char  *kickReason = NULL;
	if (params.size() == 3)
		kickReason = (params[2]).c_str();

	// find channel
	channel = findChanByName(channelName);
	if (channel == NULL) // channel doesn't exist
	{
		handleReplies(ERR_NOSUCHCHANNEL, channelName, NULL, sender);
		return ;
	}
	target = channel->findUserByNick(targetNick);
	if (target == NULL)
	{
		std::cout << "User doesn't exist in this channel" << std::endl; // test
		handleReplies(ERR_USERNOTINCHANNEL, targetNick + " " + channelName, NULL, sender);
		return ;
	}
	// check whether sender is an admin in that channel
	try
	{
		if (channel->isUserOp(sender) == false) // it means user isn't an op
		{
			std::cout << "User is not an admin" << std::endl; // test
			handleReplies(ERR_CHANOPRIVSNEEDED, channelName, NULL, sender);
			return ;
		}
		else // user is indeed an op : can proceed
			std::cout << "user successfully kicked" << std::endl; // test
	}
	catch (std::exception &e) // it means user isn't even in channel
	{
		if (!strcmp(e.what(), "ERR_NOTONCHANNEL"))
		{
			std::cout << "User is not even in the channel..." << std::endl; // test
			handleReplies(ERR_NOTONCHANNEL, channelName, NULL, sender);
			return ;
		}
	}
	kickReplies(sender, *target, *channel, kickReason);
	this->partCommand(channelName, *target, "Kicked from channel");
	// handle replies
}
