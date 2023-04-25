/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itaouil <itaouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 19:07:31 by itaouil           #+#    #+#             */
/*   Updated: 2023/04/25 21:31:33 by itaouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_irc.hpp"

Channel::~Channel( void ) { }

Channel::Channel( std::string name, Client &creator ) : _name(name)
{
	std::pair<Client, bool> newUser(creator, true);
	(this->_users).insert(newUser);
	this->_maxUsers = -1;
}

std::string Channel::getName( void ) const
{
	return (this->_name);
}

Channel::Channel(const Channel &other)
{
	this->_name = other._name;
	this->_users = other._users;
	this->_maxUsers = other._maxUsers;
	this->_topic = other._topic;
}

Channel &Channel::operator=(const Channel &other)
{
	this->_name = other._name;
	this->_users = other._users;
	this->_maxUsers = other._maxUsers;
	this->_topic = other._topic;
	return (*this);
}


int Channel::getUserLimit( void ) const
{
	return (this->_maxUsers);
}

void Channel::setUserLimit( int limit )
{
	if (limit < -1)
		throw (channelException("Invalid user limit"));
	this->_maxUsers = limit;
}

void Channel::sendMessage( std::string &message, Client const &receiver )
{
	int 			clientSocket = receiver.GetSocketFD();
	// unsigned int	msgLen = message.size();

	send(clientSocket, message.c_str(), message.size(), 0);
}

void	Channel::broadcastMessageToAll( std::string &message )
{
	std::map<Client, bool>::iterator	it;
	std::map<Client, bool>::iterator	ite;

	it = (this->_users).begin();
	ite = (this->_users).end();
	
	while (it != ite)
	{
		sendMessage(message, it->first);
		it++;
	}
}

void Channel::broadcastMessage( std::string &message, Client &sender )
{
	std::map<Client, bool>::iterator	it;
	std::map<Client, bool>::iterator	ite;

	it = (this->_users).begin();
	ite = (this->_users).end();
	
	while (it != ite)
	{
		if (it->first.GetNickname() != sender.GetNickname())
			sendMessage(message, it->first);
		it++;
	}
}

void Channel::addUser( Client user )
{
	if (static_cast<int>(this->_users.size()) == this->_maxUsers)
		throw (channelException("Channel is at full capacity."));
	
	for (size_t i = 0; i < this->_blacklist.size(); i++)
    {
        if (this->_blacklist[i] == user.GetUsername())
            throw (channelException("You are banned from this channel."));
    }
    std::pair<Client, bool> newUser(user, false);
	(this->_users).insert(newUser);
}

void Channel::removeUser( Client user )
{
	std::map<Client, bool>::iterator i;
	i = (this->_users).find(user);

	if (i != (this->_users).end())
		(this->_users).erase(i);
}

void Channel::giveOpRights( Client user )
{
	std::map<Client, bool>::iterator it;
	it = (this->_users).find(user);

	if (it != (this->_users).end())
	{
		it->second = true;
	}
}

void Channel::removeOpRights( Client user )
{
	std::map<Client, bool>::iterator it;
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

bool Channel::operator==(const Channel& rhs) const
{
	return (this->_name == rhs._name);
}

int Channel::getNbUsers( void ) const
{
	return (this->_users.size());
}

std::map<Client, bool>	const &Channel::getUsers( void ) const
{
	return (this->_users);
}

std::vector<std::string> Channel::getBlacklist( void ) const
{
	return (this->_blacklist);
}

bool Channel::operator<(const Channel& rhs) const
{
	return (this->_name < rhs._name);
}

void Channel::addToBlacklist(std::string nickname)
{
	for (size_t i = 0; i < this->_blacklist.size(); i++) // check if already in blacklist
	{
		if (this->_blacklist[i] == nickname)
			return ;
	}
	this->_blacklist.push_back(nickname);
}

void Channel::removeToBlacklist(std::string nickname)
{
	this->_blacklist.erase(std::remove(this->_blacklist.begin(), this->_blacklist.end(), nickname), this->_blacklist.end());
}

bool Channel::isInBlacklist(std::string nickname)
{
	return (std::find(this->_blacklist.begin(), this->_blacklist.end(), nickname) != this->_blacklist.end());
}

bool Channel::isUserOp( Client &user )
{
	std::map<Client, bool>::iterator 	it;
	std::map<Client, bool>::iterator 	ite = _users.end();
	std::string							targetNick = user.GetNickname();
	for (it = _users.begin(); it != ite; it++)
	{
		if (!((it->first).GetNickname()).compare(targetNick))
		{
			return (it->second);
		}
	}
	throw (channelException("ERR_NOTONCHANNEL"));
}

Client	*Channel::findUserByNick( std::string nick )
{
    Client  *user = NULL;
    std::map<Client, bool>::iterator it;
    std::map<Client, bool>::iterator ite = _users.end();

    for (it = _users.begin(); it!= ite; it++)
    {
        if (it->first.GetNickname() == nick)
        {
            user = const_cast<Client *>(&(it->first));
            break ;
        }
    }
    return (user);
}
