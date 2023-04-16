#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <exception>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <poll.h>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>
#include <algorithm>

#include "client.hpp"
#include "Channel.hpp"
#include "Server/Server.hpp"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024 // Taille du buffer de réception

// REPLIES CODES
#define RPL_TOPIC 332

// ERROR CODES
#define NAMETOOLONG 0
#define NOTENOUGHPARAMS 1
#define WRONGNAME 2
#define CHANNELALREADYEXISTS 3
#define VALIDNAME 4

#define ERR_NOSUCHCHANNEL 403
#define ERR_TOOMANYCHANNELS 405
#define ERR_NEEDMOREPARAMS 461
#define ERR_CHANNELISFULL 471
#define ERR_INVITEONLYCHAN 473
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475


// COLORS
#define bg_black        "[40m"
#define red             "[1;31m"
#define reset           "[0m"

// PROTOTYPES
std::vector<std::string> split(const std::string& s, char delimiter);
bool parseClientInfo(std::string& message, std::string& password, std::string& nickname, std::string& username);
void removeNewlinesAndDoubleSpaces(std::string& str);

void	errReplies( int code, std::string param, Client &client );