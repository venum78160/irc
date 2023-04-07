/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vl-hotel <vl-hotel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 19:07:31 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/06 20:38:20 by vl-hotel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

Channel::~Channel( void ) { }

Channel::Channel( std::string name, Client *creator ) : _name(name)
{
	std::pair<Client *, bool> newUser(creator, true);
	(this->_users).insert(newUser);
	this->_maxUsers = -1;
}

std::string Channel::getName( void ) const
{
	return (this->_name);
}

// void Channel::rename( std::string name )
// {
// 	this->_name = name;
// }

int Channel::getUserLimit( void ) const
{
	if (this->_maxUsers == -1)
		throw (channelException("No user limit set"));
	return (this->_maxUsers);
}

void Channel::setUserLimit( int limit )
{
	if (limit < -1)
		throw (channelException("Invalid user limit"));
	this->_maxUsers = limit;
}

bool	Channel::checkNameValidity( std::string &name )
{
	int	nameLen = name.size();
	if (nameLen > 50 || nameLen < 2)
		return (false);
	if (name[0] != '#')
		return (false);

	for (int i = 0; i < nameLen; i++)
	{
		if (isspace(name[i]) || name[i] == ',' || name[i] == 7)
			return (false);
	}
	return (true);
}

void Channel::sendMessage( std::string message, Client const *receiver )
{
	int 			clientSocket = receiver->GetSocketFD();
	unsigned int	msgLen = message.size();

	send(clientSocket, &message, msgLen, 0);
}

void Channel::broadcastMessage( std::string message, Client &sender )
{
	std::map<Client *, bool>::iterator	it;
	std::map<Client *, bool>::iterator	ite;

	it = (this->_users).begin();
	ite = (this->_users).end();
	
	while (it != ite)
	{
		if (it->first->GetUsername() != sender.GetUsername())
			sendMessage(message, it->first);
	}
}

void Channel::addUser( Client *user )
{
	if (this->_users.size() == this->_maxUsers)
		throw (channelException("Channel is at full capacity."));
	
	std::pair<Client *, bool> newUser(user, false);
	(this->_users).insert(newUser);
}

void Channel::removeUser( Client *user )
{
	std::map<Client *, bool>::iterator i;
	i = (this->_users).find(user);

	if (i != (this->_users).end())
		(this->_users).erase(i);
}

void Channel::giveOpRights( Client *user )
{
	std::map<Client *, bool>::iterator it;
	it = (this->_users).find(user);

	if (it != (this->_users).end())
		it->second = true;
}

void Channel::removeOpRights( Client *user )
{
	std::map<Client *, bool>::iterator it;
	it = (this->_users).find(user);

	if (it != (this->_users).end())
		it->second = false;
}

void Channel::setTopic( std::string topic )
{
	this->_topic = topic;
}

std::string	Channel::getTopic( void ) const
{
	return (this->_topic);
}
