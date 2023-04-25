/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 16:23:42 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/25 21:35:04 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_irc.hpp"

void	Server::sendPrivMsg( std::string targetNick, std::string &message, Client &sender )
{
	Client	*target = NULL;
	std::map<int , Client>::iterator it;
	std::map<int , Client>::iterator ite = _MClient.end();

	for (it = _MClient.begin(); it != ite; it++)
	{
		if (!((it->second).GetNickname()).compare(targetNick))
		{
			target = &(it->second);
			break ;
		}
	}
	if (it == ite || target == NULL) // no such client
	{
		// std::cout << "no such client" << std::endl;
		handleReplies(ERR_NOSUCHNICK, targetNick, NULL, sender);
		return ;
	}
	send(target->GetSocketFD(), message.c_str(), message.size(), 0);
}

void	Server::notifyChannel( std::string channelName, std::string &msg, Client &sender )
{
	Channel	*targetChannel = NULL;
	std::vector<Channel *>::iterator it;
	std::vector<Channel *>::iterator ite = _channels.end();

	for (it = _channels.begin(); it != ite; it++)
	{
		if (!((*it)->getName()).compare(channelName))
		{
			targetChannel = *it;
			break ;
		}
	}
	if (it == ite || targetChannel == NULL) // channel not found
	{
		// std::cout << "no such channel" << std::endl;
		handleReplies(ERR_NOSUCHNICK, channelName, NULL, sender);
		return ;
	}
	targetChannel->broadcastMessage(msg, sender);
}

void	Server::ft_privMsg( std::string command, Client &sender )
{
	if (command.find(" ") == std::string::npos)
	{
		handleReplies(ERR_NORECIPIENT, "privmsg", NULL, sender);
		return ;
	}
	std::vector<std::string> params = splitOnFirstSpace(command);
	if (params.size() < 2) // check if there is enough params, there should be 2: first is target second is message
	{
		handleReplies(ERR_NOTEXTTOSEND, "privmsg", NULL, sender);
		return ;
	}
	std::string senderNick = sender.GetNickname();
	std::string	senderUser = sender.GetUsername();
	std::string message = ":" + sender.getFullId() + " PRIVMSG " + command;
	std::string target = params[0];

	// check validity of target
	// checkValidity(target);
	if (target[0] == '#')
		notifyChannel(target, message, sender);
	else if (!target.compare("!bot"))
	{
		std::string botQuery = command.substr(command.find_first_of(":") + 1);
		start_bot(botQuery, sender);
	}
	else
	{
		sendPrivMsg(target, message, sender);
	}
}
