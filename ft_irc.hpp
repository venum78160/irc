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

