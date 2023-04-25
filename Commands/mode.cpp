#include "../ft_irc.hpp"

bool Server::channelExist(std::string channelName)
{
    std::vector<Channel *> channels = this->getServerChannels();
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if ((*it)->getName() == channelName)
            return (true);
    }
    return (false);
}

bool Server::nicknameExist(std::string nickname)
{
	for (std::map<int, Client>::iterator it = _MClient.begin(); it != _MClient.end(); ++it)
	{
		if (it->second.GetNickname() == nickname)
			return (true);
	}
	return (false);
}

Channel *Server::getChannelByName(std::string channelName)
{
    for (size_t i = 0; i < _channels.size(); i++)
    {
        if (_channels[i]->getName() == channelName)
            return (_channels[i]);
    }
    return (NULL); // never happens because we check if channel exist before
}

Client    &Server::getClientByNickname(std::string nickname)
{
    for (std::map<int, Client>::iterator it = _MClient.begin(); it != _MClient.end(); ++it)
    {
        if (it->second.GetNickname() == nickname)
            return (it->second);
    }
    static Client client;
    return (client); // never happens because we check if nickname exist before
}

void    Server::executeModeChannels(Client &client, std::vector<std::string> &params)
{
    Channel *channel = this->getChannelByName(params[1]);
    std::map<Client, bool> users = channel->getUsers();
    if (client.GetServername() != channel->getName()) {
        std::string reply = ":127.0.0.1 442 " + client.GetNickname() + " " + channel->getName() + " :You're not on that channel\r\n";
        send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
        return;
    }
    if ((params[2][0] != '+' && params[2][0] != '-') || (params[2].size() - 1 != params.size() - 3))
    {
        std::string reply = ":127.0.0.1 472 " + client.GetNickname() + " " + client.GetNickname() + " " + params[2][0] + " :is unknown mode char to me\r\n";
        send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
        return ;
    }
    if (users[client] == false)
    {
        handleReplies(ERR_CHANOPRIVSNEEDED, channel->getName(), NULL, client);
        return ;
    }
    if (params[2][0] == '+')
    {
        for (size_t i = 1; i < params[2].size(); i++) {
            if (params[2][i] == 'o') {
				if (params[i + 2].find("\r\n") != std::string::npos)
					params[i + 2].substr(0, params[i + 2].size() - 2);
				channel->giveOpRights(getClientByNickname(params[i + 2]));
				std::string reply = ":" + client.GetNickname() + " MODE " + channel->getName() + " +o " + params[i + 2] + "\r\n";
				channel->broadcastMessageToAll(reply);
				reply = ":127.0.0.1 324 " + client.GetNickname() + " " + channel->getName() + " :You're now channel operator\r\n";
				send(getClientByNickname(params[i + 2]).GetSocketFD(), reply.c_str(), reply.size(), 0);
				handleReplies(RPL_NAMREPLY, "", channel, client);
                // else {
                //     std::string reply = ":127.0.0.1 401 " + client.GetNickname() + " " + params[i + 2] + " :No such nick/channel\r\n";
                //     std::cout << "nickname doesn't exists :" << reply << std::endl;
				// 	send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);

            }
            if (params[2][i] == 'l') {
                try {
                    int limit = std::atoi(params[i + 2].c_str());
                    if (limit > 0 && limit < 100) {
                        channel->setUserLimit(limit);
                        std::string reply = ":"+ client.GetNickname() +  " MODE "  + channel->getName() + " +l\r\n";
                        send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                    }
                     else {
                        std::string reply = ":127.0.0.1 461 " + client.GetNickname() + " :Invalid mode parameter\r\n";
                        send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                        return;
                    }
                }
                catch(const std::exception& e)
                {
                    std::string reply = ":127.0.0.1 461 " + client.GetNickname() + " :Invalid mode parameter\r\n";
                    send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                    return;
                }
            }
            if (params[2][i] == 'b') {
                if (!channel->isInBlacklist(params[i + 2])) {
                    channel->addToBlacklist(params[i + 2]);
					params[i + 2].erase(params[i + 2].size() - 2, 2);
                    std::string reply = ":"+ client.GetNickname() +  " MODE "  + channel->getName() + " +b\r\n";
					// std::cout << "reply : " << reply << std::endl;
					send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
					this->partCommand(channel->getName(), getClientByNickname(params[i + 2]), "Got banned");
				}
                else {
					std::string reply = ":127.0.0.1 442" + client.GetNickname() + " " + channel->getName() + " :User is already banned\r\n";
                    send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                }
            }
        }
    }
    else
    {
        for (size_t i = 1; i < params[2].size(); i++) {
            if (params[2][i] == 'o') {
                    channel->removeOpRights(getClientByNickname(params[i + 2]));
                    std::string reply = ":" + client.GetNickname() + " MODE " + channel->getName() + " -o " + params[i + 2] + "\r\n";
                    channel->broadcastMessageToAll(reply);
					reply = ":127.0.0.1 324 " + client.GetNickname() + " " + channel->getName() + " :You're no longer channel operator\r\n";
					send(getClientByNickname(params[i + 2]).GetSocketFD(), reply.c_str(), reply.size(), 0);
					handleReplies(RPL_NAMREPLY, "", channel, client);
            }
            if (params[2][i] == 'l') {
                channel->setUserLimit(-1);
                std::string reply = ":"+ client.GetNickname() +  " MODE "  + channel->getName() + " -l\r\n";
                send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
            }
            if (params[2][i] == 'b') {
                if (channel->isInBlacklist(params[i + 2])) {
                    channel->removeToBlacklist(params[i + 2]);
                    std::string reply = ":"+ client.GetNickname() +  " MODE "  + channel->getName() + " -b\r\n";
        			send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                }
                else
                {
                    std::string reply = ":127.0.0.1 368 " + client.GetNickname() + " :is not banned\r\n";
                    send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                }
            }
        }
    }
}

void Server::executeModeUsers(Client &client, std::vector<std::string> &params)
{
    Client &user = getClientByNickname(params[1]);
    if (user.GetServername() == "")
        return ;
    Channel *channel = getChannelByName(user.GetServername());
    std::map<Client, bool> users = channel->getUsers();
    if (params[2][0] != '+' && params[2][0] != '-')
        return ;
    if (params[2].size() - 1 != params.size() - 3)
        return ;
    if (params[2][0] == '+')
    {
        for (size_t i = 1; i < params[2].size(); i++)
        {
            if (params[2][i] == 'o')
            {
                if (users[client] == true)
                {
                    channel->giveOpRights(user);
                    std::string reply = ":" + client.GetNickname() + " MODE " + channel->getName() + " +o " + params[i + 2] + "\r\n";
					channel->broadcastMessageToAll(reply);
                }
                else
                    handleReplies(ERR_CHANOPRIVSNEEDED, channel->getName(), NULL, client);
            }
        }
    }
    else
    {
        for (size_t i = 1; i < params[2].size(); i++)
        {
            if (params[2][i] == 'o')
            {
                if (users[client] == true)
                {
                    channel->removeOpRights(user);
                    std::string reply = ":" + client.GetNickname() + " MODE " + channel->getName() + " -o " + params[i + 2] + "\r\n";
                    channel->broadcastMessageToAll(reply);
					reply = ":127.0.0.1 324 " + client.GetNickname() + " " + channel->getName() + " :You're no longer channel operator\r\n";
					send(getClientByNickname(params[i + 2]).GetSocketFD(), reply.c_str(), reply.size(), 0);
            		handleReplies(RPL_NAMREPLY, "", channel, client);
				}
				else
					handleReplies(ERR_CHANOPRIVSNEEDED, channel->getName(), NULL, client);
			}
		}
	}
	return ;
}