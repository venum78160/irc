#pragma once

#include "ft_irc.hpp"

class	Client;

class	Channel
{

private:
	std::string					_name; // name of the channel
	std::map<Client *, bool> 	_users; // list of users, coupled with a boolean that is true if user is an operator on this channel
	std::vector<std::string>	_userNicks; // list of users by their nicknames, useful for NAMES command
	int							_maxUsers; // equals -1 if no limit is set
	std::string					_topic;

	Channel( void ); // class must be initialized with a name and a creator, no default constructor

public:
	Channel( std::string name, Client *creator );
	~Channel( void );

	// Accessors
	std::string 	getName( void ) const;
	// void			rename( std::string name );
	int				getUserLimit( void ) const;
	void			setUserLimit( int limit );
	std::string		getTopic( void ) const;
	void			setTopic( std::string topic );

	bool	checkNameValidity( std::string &name ); // checks if channel name is valid. should later be included in Server class as well

	void	broadcastMessage( std::string message, Client &sender );
	void	sendMessage( std::string message, Client const *receiver );

	void	removeUser( Client *user );
	void	addUser( Client *user );

	void	giveOpRights( Client *user );
	void	removeOpRights( Client *user );

	class	channelException : public std::exception
	{
	private:
		std::string	_error;

	public:
		channelException();
		channelException( std::string const &message ) throw() : _error(message) {};
		virtual ~channelException( void ) throw() {};

		virtual const char* what( void ) const throw()
		{
			return ((this->_error).c_str());
		};
	};
};
