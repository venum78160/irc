/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 20:59:42 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/25 21:35:13 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_irc.hpp"

void	Server::ft_topic( std::string &msg, Client &sender )
{
	if (msg.size() < 8)
	{
		handleReplies(ERR_NEEDMOREPARAMS, "TOPIC", NULL, sender);
		return ;
	}
	std::string channelName = msg.substr(6, msg.find_first_of(" ", 6) - 6);
	std::string topic = msg.substr(msg.find_first_of(" ", 7) + 2);

	Channel		*channel = NULL;
	std::string	senderNick = sender.GetNickname();
	channel = findChanByName(channelName);
	if (channel == NULL)
	{
		handleReplies(ERR_NOSUCHCHANNEL, channelName, NULL, sender);
		return ;
	}
	channel->setTopic(topic);
	if (topic.size() == 0)
	{
		handleReplies(RPL_NOTOPIC, channelName, NULL, sender);
		return ;
	}
	handleReplies(RPL_TOPIC, channelName, channel, sender);
}
