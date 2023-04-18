/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 16:23:42 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/18 18:50:16 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_irc.hpp"

void	Server::privMsgCommand( std::string command, Client &sender )
{
	Client						target;
	std::string 				targetNick;
	std::string					message;
	std::vector<std::string> 	args = splitStr(command, ' ');

	if (command.find("PRIVMSG") == std::string::npos || command.size() < 9) // function protection, although it should never be verified
		return ;
	if (args.size() < 3)
	{
		// send error message about wrong input
		return ;
	}
	targetNick = args[1]; // retrieving target's nickname from the command
	
	std::map<int, Client>::iterator it;
	std::map<int, Client>::iterator ite = _MClient.end(); // retrieving target and making sure it exists
	for (it = _MClient.begin(); it != ite; *it++)
		if ((it->second).GetNickname() == targetNick)
		{
			target = it->second;
			break ;
		}
	if (it == ite) // in case target doesn't exist
	{
		// send error message about non-existent user
		return ;
	}
	message = sender.getFullId() + " " + command;
	send(target.GetSocketFD(), &message, sizeof(message), 0);
}

void	Server::ft_privMsg( std::string command, Client &sender )
{
	
}
