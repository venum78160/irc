/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 15:48:48 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/25 21:31:45 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// A implémenter à l'intérieur de la classe Server plus tard

#include "ft_irc.hpp"

// void	commandsReturns( std::string &param, std::string &reply )
// {
// 	if (!param.compare("NICK"))
		
// }

void	sendReply( Client &client, std::string reply)
{
	send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
}

void	successReplies( int code, Channel &param, std::string &reply )
{
	if (code == RPL_TOPIC)
	{
		std::string chan = param.getName();
		std::string topic = param.getTopic();
		reply.append(chan + " :" + topic + "\r\n");
		// param.broadcastMessageToAll(reply);
	}
	else if (code == RPL_NOTOPIC)
	{
		std::string chan = param.getName();
		reply.append(chan + " :No topic is set\r\n");
		// param.broadcastMessageToAll(reply);
	}
	else if (code == RPL_NAMREPLY)
	{
		std::string chan = param.getName();
		reply.append("= " + chan + " :");
		std::map<Client, bool> users = param.getUsers();
		std::map<Client, bool>::iterator it;
		std::map<Client, bool>::iterator ite = (users.end());
		// look through users and append each user's nickname to reply
		for (it = users.begin(); it != ite; it++)
		{
			if (it->second == true)
				reply.append("@" + it->first.GetNickname() + " ");
			else
				reply.append(it->first.GetNickname() + " ");
		}
		reply.append("\r\n");
	}
	else if (code == RPL_ENDOFNAMES)
	{
		std::string chan = param.getName();
		reply.append(chan + " :End of /NAMES list\r\n");
		// std::cout << "RPLENDOFNAMES reply: " << reply << std::endl;
	}
}

void	channelErrors( int code, std::string &param, std::string &reply )
{
	//remplacer par un switch
	if (code == 403) // Channel doesn't exist
		reply.append(param + ":No such channel\r\n");
	else if (code == 471) // Channel is full
		reply.append(param + ":Cannot join channel (+l)\r\n");
	else if (code == 473) // Channel is invite-only
		reply.append(param + ":Cannot join channel (+i)\r\n");
	else if (code == 474) // Banned
		reply.append(param + ": Cannot join channel (+b)\r\n");
	else if (code == ERR_NOTONCHANNEL)
		reply.append(param + ":You're not on that channel\r\n");
	else if (code == ERR_USERNOTINCHANNEL)
		reply.append(param + ":They aren't on that channel\r\n");
	else if (code == ERR_CHANOPRIVSNEEDED)
		reply.append(param + ":You're not channel operator\r\n");
}

void	parsingErrors( int code, std::string &param, std::string &reply )
{
	if (code == 461)
		reply.append(param + ":Not enough parameters\r\n");
	else if (code == ERR_NORECIPIENT)
		reply.append(":No recipient given\r\n");
	else if (code == ERR_NOSUCHNICK)
		reply.append(param + ":No such nick/channel\r\n");
	else if (code == ERR_NOTEXTTOSEND)
		reply.append(":No text to send\r\n");
	else if (code == ERR_NONICKNAMEGIVEN)
		reply.append(":No nickname given\r\n");
	else if (code == ERR_ERRONEUSNICKNAME)
		reply.append(param + ":Erroneous nickname\r\n");
	else if (code == ERR_NICKNAMEINUSE)
		reply.append(param + ":Nickname is already in use\r\n");
}


void	Server::handleReplies( int code, std::string param, Channel *chan, Client &target )
{
	std::string reply = ":127.0.0.1 " + ft_itoa(code) + " " + target.GetNickname() + " ";
	// RETOURS DE COMMANDES
	if (code == RPL_TOPIC || code == RPL_NOTOPIC || code == RPL_NAMREPLY || code == RPL_ENDOFNAMES)
	{ 
		successReplies(code, *chan, reply);
		// if (code == RPL_TOPIC || code == RPL_NOTOPIC)
		// 	return ;
	}
	// ERREURS
	else if (code == 403 || code == 405 || code == 471 || code == 473 ||
	code == 474 || code == 475 || code == ERR_NOTONCHANNEL || code == ERR_USERNOTINCHANNEL ||
	code == ERR_CHANOPRIVSNEEDED)
		channelErrors(code, param, reply);
	else if (code == 461 || code == ERR_NOSUCHNICK || code == ERR_NOTEXTTOSEND || code == ERR_NORECIPIENT || code == RPL_ENDOFNAMES)
		parsingErrors(code, param, reply);
	// if (code != RPL_TOPIC && code != RPL_NOTOPIC)
	sendReply(target, reply);
}
