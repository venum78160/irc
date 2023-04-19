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

Client    Server::getClientByNickname(std::string nickname)
{
    for (std::map<int, Client>::iterator it = _MClient.begin(); it != _MClient.end(); ++it)
    {
        if (it->second.GetNickname() == nickname)
            return (it->second);
    }
    return (Client()); // never happens because we check if nickname exist before
}

void    Server::executeModeChannels(Client &client, std::vector<std::string> &params)
{
    Channel *channel = this->getChannelByName(params[1]);
    if (params[2][0] != '+' && params[2][0] != '-')
        return ;
    // check if +ooo 3 nicknames must be there
    if (params[2].size() - 1 != params.size() - 3)
        return ;
    if (params[2][0] == '+')
    {
        for (size_t i = 1; i < params[2].size(); i++) {
            if (params[2][i] == 'o') {
                if (nicknameExist(params[i + 2]))
                    channel->giveOpRights(getClientByNickname(params[i + 2]));
                else {
                    std::string reply = ": " + params[i + 2] + " :No such nick/channel\r\n";
                    send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                }
            }
            if (params[2][i] == 'l') {
                try {
                    int limit = std::atoi(params[i + 2].c_str());
                    if (limit > 0 && limit < 100)
                        channel->setUserLimit(limit);
                    else {
                        std::string reply = ": 461 " + client.GetNickname() + " :Invalid mode parameter\r\n";
                        send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                        return;
                    }
                }
                catch(const std::exception& e)
                {
                    std::string reply = ": 461 " + client.GetNickname() + " :Invalid mode parameter\r\n";
                    send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                    return;
                }
            }
            if (params[2][i] == 'b') {
                std::map<Client, bool> users = channel->getUsers();
                if (users[client] == true)
                    channel->addToBlacklist(params[i + 2]);
                else
                {
                    std::string reply = ": 482 " + client.GetNickname() + " :You're not channel operator\r\n";
                    send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                    return;
                }
            }
        }
    }
    else
    {
        for (size_t i = 1; i < params[2].size(); i++) {
            if (params[2][i] == 'o') {
                if (nicknameExist(params[i + 2]))
                    channel->removeOpRights(getClientByNickname(params[i + 2]));
                else {
                    std::string reply = ": " + params[i + 2] + " :No such nick/channel\r\n";
                    send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                }
            }
            if (params[2][i] == 'l')
                channel->setUserLimit(-1);
            if (params[2][i] == 'b') {
                std::map<Client, bool> users = channel->getUsers();
                if (users[client] == true)
                    channel->removeToBlacklist(params[i + 2]);
                else
                {
                    std::string reply = ": 482 " + client.GetNickname() + " :You're not channel operator\r\n";
                    send(client.GetSocketFD(), reply.c_str(), reply.size(), 0);
                    return;
                }
            }
        }
    }
}

void Server::executeModeUsers(Client &client, std::vector<std::string> &params)
{
    (void)client;
    (void)params;
    return ;
}