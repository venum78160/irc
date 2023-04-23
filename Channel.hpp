#pragma once

#include "ft_irc.hpp"

class	Client;

class	Channel
{

private:
	std::string					_name; // name of the channel
	std::map<Client, bool>		_users; // list of users, coupled with a boolean that is true if user is an operator
    std::vector<std::string>	_userNicks;
	int							_maxUsers;
	std::string					_topic;
	std::vector<std::string>	_blacklist;
	Channel( void );

public:
	Channel( std::string name, Client &creator );
	~Channel( void );
	Channel &operator=(const Channel &other);
	Channel (const Channel &other);
	bool operator==(const Channel& other) const;
	bool operator<(const Channel& other) const;

	// Utils
	bool	isUserOp( Client &user );

	// Accessors
	std::string 	getName( void ) const;
	int 			getUserLimit( void ) const;
	void			setUserLimit( int limit );

	int				getNbUsers( void ) const;
	Client			*findUserByNick( std::string nick );

	std::map<Client, bool>	const &getUsers( void ) const;

	void	removeUser( Client user );
	void	addUser( Client user );
	void	addToBlacklist(std::string nickname);
	void	removeToBlacklist(std::string nickname);
	bool	isInBlacklist(std::string nickname);

	void	giveOpRights( Client user );
	void	removeOpRights( Client user );

	void		setTopic( std::string topic );
	std::string	getTopic( void ) const;

	std::vector<std::string>	getBlacklist( void ) const;

	// Messages handling
	void	broadcastMessageToAll( std::string &message );
	void	broadcastMessage( std::string &message, Client &sender );
	void	sendMessage( std::string &message, Client const &receiver );

	// Exceptions
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
