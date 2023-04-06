#pragma once

#include <iostream>
#include <string>
#include <exception>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <poll.h>
#include <vector>
#include <map>

#include "client.hpp"
#include "serveur.hpp"
#include "Channel.hpp"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 512 // Taille du buffer de réception

// ERROR CODES
#define NAMETOOLONG 0
#define NOTENOUGHPARAMS 1
#define WRONGNAME 2
#define CHANNELALREADYEXISTS 3
#define VALIDNAME 4