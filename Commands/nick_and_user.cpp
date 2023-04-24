/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick_and_user.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 17:29:26 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/24 19:23:10 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_irc.hpp"

bool	Server::checkNickValidity( std::string &nick )
{
	if (nick.size() == 0)
		return (false);
	if (nick.size() > 9)
		return (false);
	if (nick.find_first_of("abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos)
		return (false);
	return (true);
}

void	Server::ft_nick( std::string &message, Client &client )
{
	std::transform(message.begin(), message.end(), message.begin(), ::tolower);
	std::string nick = message.substr(5);
	if (nick.size() == 0)
	{
		handleReplies(ERR_NONICKNAMEGIVEN, "", NULL, client);
		return ;
	}
	if (checkNickValidity(nick) == false)
	{
		handleReplies(ERR_ERRONEUSNICKNAME, nick, NULL, client);
		return ;
	}
	if (nicknameExist(nick) == true)
	{
		handleReplies(ERR_NICKNAMEINUSE, nick, NULL, client);
		return ;
	}

	client.SetNickname(nick);
	std::string rpl = ":" + client.getFullId() + + " NICK " + nick + "\r\n";
	client.setFullId(nick + "!" + client.GetUsername() + "@" + "HOST");
	send(client.GetSocketFD(), rpl.c_str(), rpl.size(), 0);
}

// bool	Server::checkUserValidity( std::string &user )
// {
// 	if (user.size() == 0)
// 		return (false);
// 	if (user.size() > 9)
// 		return (false);
// 	if (user.find_first_of("abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ") == std::string::npos)
// 		return (false);
// 	return (true);
// }

void	Server::ft_user( std::string &message, Client &sender )
{
	std::transform(message.begin(), message.end(), message.begin(), ::tolower);
	std::string user = message.substr(5);
	if (user.size() == 0)
	{
		handleReplies(ERR_NEEDMOREPARAMS, "USER", NULL, sender);
		return ;
	}
	sender.SetUsername(user);
	std::string rpl = ":" + sender.getFullId() + + " NICK " + user + "\r\n";
	sender.setFullId(user + "!" + sender.GetUsername() + "@" + "HOST");
	send(sender.GetSocketFD(), rpl.c_str(), rpl.size(), 0);
}
