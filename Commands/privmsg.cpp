/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42nice.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 16:23:42 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/20 00:34:26 by itaouil          ###   ########.fr       */
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

void	Server::sendPrivMsg( std::string targetNick, std::string message )
{
	Client	*target = nullptr;
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
	if (it == ite || target == nullptr) // no such client
	{
		// send error
		return ;
	}
	send(target->GetSocketFD(), &message, sizeof(message), 0);
}

void	Server::notifyChannel( std::string channelName, std::string &msg, Client &sender )
{
	Channel	*targetChannel = nullptr;
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
	if (it == ite || targetChannel == nullptr) // channel not found
	{
		// send error
		return ;
	}
	
	targetChannel->broadcastMessage(msg, sender);
}

void	Server::ft_privMsg( std::string command, Client &sender )
{
	std::transform(command.begin(), command.end(), command.begin(), ::tolower);

	// std::cout << "privmsg message: " << command << std::endl; // test only, to delete later
	std::vector<std::string> params = splitStr(command, ' ');
	if (params.size() < 2) // check if there is enough params
	{
		// std::cout << "not enough params" << std::endl; // test only, to delete later
		handleReplies(ERR_NEEDMOREPARAMS, "join", NULL, sender);
		return ;
	}
	std::string message = params.back();
	// FORMATER LE MESSAGE !!!
	std::vector<std::string> targets = splitStr(params[1], ','); // split targets in case there is more than one
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator ite = targets.end();

	for (it = targets.begin(); it != ite; it++)
	{
		// check validity of target
		// checkValidity(*it);
		if ((*it)[0] == '#')
			notifyChannel(*it, message, sender);
		else
			sendPrivMsg(*it, message);
	}
}
