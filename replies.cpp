/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 15:48:48 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/18 17:38:56 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// A implémenter à l'intérieur de la classe Server plus tard

#include "ft_irc.hpp"

void	successReplies( int code, Channel &param, std::string &reply )
{
	if (code == RPL_TOPIC)
	{
		std::string chan = param.getName();
		std::string topic = param.getTopic();
		reply.append(chan + " :" + topic + "\r\n");
	}
	else if (code == RPL_NAMREPLY)
	{
		std::string chan = param.getName();
		reply.append(chan + " :");
		std::map<Client, bool> users = param.getUsers();
		std::map<Client, bool>::iterator it;
		std::map<Client, bool>::iterator ite = (users.end());
		// look through users and append each user's nickname to reply
		for (it = users.begin(); it != ite; it++)
		{
			if (it->second == true)
				reply.append(" @" + it->first.GetNickname());
			else
				reply.append(" " + it->first.GetNickname());
		}
		reply.append("\r\n");
	}
}

void	channelErrors( int code, std::string &param, std::string &reply )
{
	//remplacer par un switch
	if (code == 403) // Channel doesn't exist
		reply.append(param + ":No such channel\r\n");
	// else if (code == 405) // pas nécessaire à implémenter ?
		// reply.append(param + ":You have joined too many channels\r\n");
	else if (code == 471) // Channel is full
		reply.append(param + ":Cannot join channel (+l)\r\n");
	else if (code == 473) // Channel is invite-only
		reply.append(param + ":Cannot join channel (+i)\r\n");
	else if (code == 474) // Banned
		reply.append(param + ": Cannot join channel (+b)\r\n");
	// else if (code == 475) // Channel password is wrong -> not necessary to implement ?
		// reply.append(param + ": Cannot join channel (+k)\r\n");
}

void	parsingErrors( int code, std::string &param, std::string &reply )
{
	if (code == 461)
		reply.append(param + ":Not enough parameters\r\n");
}

void	sendReply( Client &client, std::string reply )
{
	send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
}


void	Server::handleReplies( int code, std::string param, Channel *chan, Client &client )
{
	std::string reply = ":127.0.0.1 " + ft_itoa(code) + " " + client.GetNickname() + " ";
	if (code == 403 || code == 405 || code == 471 || code == 473 ||
	code == 474 || code == 475)
		channelErrors(code, param, reply);
	else if (code == 461)
		parsingErrors(code, param, reply);
	else if (code == RPL_TOPIC || code == RPL_NAMREPLY)
		successReplies(code, *chan, reply);
	sendReply(client, reply);
}
