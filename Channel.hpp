#pragma once

#include "ft_irc.hpp"

class	Client;

class	Channel
{

private:
	std::string					_name; // name of the channel
	std::map<Client, bool> 	    _users; // list of users, coupled with a boolean that is true if user is an operator
    std::vector<std::string>    _userNicks;
	int				            _maxUsers;
	std::string					_topic;
	Channel( void );

public:
	Channel( std::string name, Client &creator );
	~Channel( void );
    Channel &operator=(const Channel &other);
    Channel (const Channel &other);
    bool operator==(const Channel& other) const;
    bool operator<(const Channel& other) const;

	// Accessors
	std::string 	getName( void ) const;
	// void			rename( std::string name );
	int	getUserLimit( void ) const;
	void			setUserLimit( int limit );

	bool	checkNameValidity( std::string &name ); // checks if channel name is valid. should later be included in Server class as well

	void	broadcastMessage( std::string message, Client sender );
	void	sendMessage( std::string message, Client const receiver );

	void	removeUser( Client user );
	void	addUser( Client user );

	void	giveOpRights( Client user );
	void	removeOpRights( Client user );

	void		setTopic( std::string topic );
	std::string	getTopic( void ) const;

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
