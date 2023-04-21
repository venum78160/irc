/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42nice.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 16:23:42 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/21 04:11:04 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_irc.hpp"

// void	Server::privMsgCommand( std::string command, Client &sender )
// {
// 	Client						target;
// 	std::string 				targetNick;
// 	std::string					message;
// 	std::vector<std::string> 	args = splitStr(command, ' ');

// 	if (command.find("PRIVMSG") == std::string::npos || command.size() < 9) // function protection, although it should never be verified
// 		return ;
// 	if (args.size() < 3)
// 	{
// 		// send error message about wrong input
// 		return ;
// 	}
// 	targetNick = args[1]; // retrieving target's nickname from the command
	
// 	std::map<int, Client>::iterator it;
// 	std::map<int, Client>::iterator ite = _MClient.end(); // retrieving target and making sure it exists
// 	for (it = _MClient.begin(); it != ite; *it++)
// 		if ((it->second).GetNickname() == targetNick)
// 		{
// 			target = it->second;
// 			break ;
// 		}
// 	if (it == ite) // in case target doesn't exist
// 	{
// 		// send error message about non-existent user
// 		return ;
// 	}
// 	message = sender.getFullId() + " " + command;
// 	send(target.GetSocketFD(), &message, sizeof(message), 0);
// }

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
		std::cout << "no such channel" << std::endl;
		handleReplies(ERR_NOSUCHNICK, channelName, NULL, sender);
		return ;
	}
	
	targetChannel->broadcastMessage(msg, sender);
}

void	Server::ft_privMsg( std::string command, Client &sender )
{
	std::cout << "privmsg message: [" << command << "]" << std::endl; // test only, to delete later
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
	// if (message[message.size() - 2] != '\n')
	// 	message.insert(message.size() - 1, "\n", 0, 1);
	std::string target = params[0];

	// std::cout << "target = [" << target << "]" << std::endl;
	// std::cout << "formated message = [" << message << "]" << std::endl;

	// check validity of target
	// checkValidity(target);
	if (target[0] == '#')
		notifyChannel(target, message, sender);
	else
		sendPrivMsg(target, message, sender);
}
