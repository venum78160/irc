/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 15:48:48 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/16 16:15:30 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// A implémenter à l'intérieur de la classe Server plus tard

#include "ft_irc.hpp"

std::string	channelErrors( int code, std::string &param )
{
	std::string reply;
	//remplacer par un switch
	if (code == 403) // Channel doesn't exist
		reply = param + ":No such channel\r\n";
	// else if (code == 405) // pas nécessaire à implémenter ?
		// reply = param + ":You have joine too many channels\r\n";
	else if (code == 471) // Channel is full
		reply = param + ":Cannot join channel (+l)\r\n";
	else if (code == 473) // Channel is invite-only
		reply = param + ":Cannot join channel (+i)\r\n";
	else if (code == 474) // Banned
		reply = param + ": Cannot join channel (+b)\r\n";
	// else if (code == 475) // Channel password is wrong -> not necessary to implement ?
		// reply = param + ": Cannot join channel (+k)\r\n";
	return (reply);
}

std::string	parsingErrors( int code, std::string &param )
{
	std::string reply;
	if (code == 461)
		reply = param + ":Not enough parameters\r\n";
	return (reply);
}

void	sendReply( Client &client, std::string reply )
{
	send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
}

void	errReplies( int code, std::string param, Client &client )
{
	std::string reply;
	if (code == 403 || code == 405 || code == 471 || code == 473 ||
	code == 474 || code == 475)
		reply = channelErrors(code, param);
	else if (code == 461)
		reply = parsingErrors(code, param);
	sendReply(client, reply);
}
