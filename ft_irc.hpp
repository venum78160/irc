#pragma once

#include <iostream>
#include <string>
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
#define BUFFER_SIZE 512 // Taille du buffer de réception

// ERROR CODES
#define NAMETOOLONG 0
#define NOTENOUGHPARAMS 1
#define WRONGNAME 2
#define CHANNELALREADYEXISTS 3
#define VALIDNAME 4

#define bg_black        "[40m"
#define red             "[1;31m"
#define reset           "[0m"