/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42nice.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 01:44:45 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/20 02:23:51 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_irc.hpp"

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
	// we need a vector with 3 args max : first one is the channel, second one is the user, third and optional one is the message that can contain spaces

	std::vector<std::string>	params = splitThreeArgs(command);
	if (params.size() < 2) // missing arguments
	{
		// send error
		return ;
	}

	Channel		*channel = NULL;
	std::string	senderNick = sender.GetNickname();
	std::string channelName = params[0];
	std::string	targetNick = params[1];
	std::string	kickReason;
	if (params.size() == 3)
		kickReason = params[2];

	// find channel
	std::vector<Channel *>::iterator	it;
	std::vector<Channel *>::iterator	ite = _channels.end();
	for (it = _channels.begin(); it != ite; it++)
	{
		if (!((*it)->getName()).compare(channelName))
		{
			channel = *it;
			break ;
		}
	}
	if (it == ite || channel == NULL) // channel not found in list
	{
		// send error
		return ;
	}
	// find target
	std::map<int, Client>::iterator	it2;
	std::map<int, Client>::iterator	ite2 = _channels.end();
	for (it2 = _MClient.begin(); it != ite; it++)
	{
		if ((it->second).GetNickname() == targetNick)
		{
			Client target = (it->second);
			break ;
		}
	}
	// check whether sender is an admin in that channel 
	
	// remove target from channel
	channel->removeUser(target);

	// handle replies
}

// int main()
// {
// 	std::string tester  = "Arg1 Arg2 ArgA B C";
// 	std::vector<std::string> myvector = splitArgs(tester);

// 	std::vector<std::string>::iterator it = myvector.begin();
// 	std::vector<std::string>::iterator ite = myvector.end();

// 	while (it != ite)
// 	{
// 		std::cout << *it << std::endl;
// 		it++;
// 	}
// }