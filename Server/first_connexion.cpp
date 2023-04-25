
#include "../ft_irc.hpp"


std::vector<std::string> split(const std::string& s, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

void removeNewlinesAndDoubleSpaces(std::string& str)
{
	// Supprimer les retours à la ligne (\r et \n)
	size_t pos = str.find_first_of("\r\n");
	while (pos != std::string::npos) {
		str.replace(pos, 1, " ");
		pos = str.find_first_of("\r\n", pos);
	}

	// Supprimer les doubles espaces
	pos = str.find("  ");
	while (pos != std::string::npos) {
		str.erase(pos, 1);
		pos = str.find("  ", pos);
	}
}

bool parseClientInfo(std::string& message, std::string& password, std::string& nickname, std::string& username)
{
	std::cout << "[parse_info]:" << message << "|"<< std::endl;
	removeNewlinesAndDoubleSpaces(message);
	std::cout << "[parse_info]2:" << message << "|"<< std::endl;
	std::vector<std::string> tokens = split(message, ' ');
	bool pass_found = false, nick_found = false, user_found = false;

	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
		if (*it == "PASS" && !pass_found) {
			password = *(++it);
			pass_found = true;
		}
		else if (*it == "NICK" && !nick_found) {
			nickname = *(++it);
			nick_found = true;
		}
		else if (*it == "USER" && !user_found) {
			username = *(++it);
			user_found = true;
		}
	}

	return (pass_found && nick_found && user_found);
}

std::string Server::recvAllData(int clientSocket)
{
	std::string data;
	char buffer[BUFFER_SIZE];
	int bytes_read;

	do {
		memset(buffer, 0, BUFFER_SIZE);
		bytes_read = recv(clientSocket, buffer, BUFFER_SIZE, 0);
		if (bytes_read == -1 || bytes_read == 0) {
			std::cerr << "Error receiving data from client " << clientSocket << std::endl;
			removeClient(clientSocket);
			return "DISCONNECTED";
		}
		data += buffer;
	} while (bytes_read == BUFFER_SIZE);

	return data;
}

void Server::handleFirstConnection(int clientSocket)
{
	//recv NICK and PASS and then reply with good format
	std::cout << "[handleconnection] :" << clientSocket << "|" << std::endl;
	std::string message = recvAllData(clientSocket);
	if (message == "DISCONNECTED")
		return ;
	std::cout << "Message reçu : |" << message << "|" <<std::endl;
	if (is_good_infos(message, clientSocket) == false)
		return;
	std::string password, nickname, username;
	parseClientInfo(message, password, nickname, username);
	std::cout << "password = " << password << ", nickname = " << nickname << ", username = " << username << std::endl;
	if (password == _password)
	{
		for (std::map<int, Client>::iterator it = _MClient.begin(); it != _MClient.end(); ++it)
		{
			if (it->second.GetNickname() == nickname)
			{
				std::string reply = ":127.0.0.1 433 " + nickname + " :Nickname is already in use\r\n";
				send(clientSocket, reply.c_str(), reply.size(), 0);
				return;
			}
		}
		//inserer le client dans la map
		_MClient.insert(std::make_pair(clientSocket, Client()));
		_MClient[clientSocket].SetSock_fd(clientSocket);
		_MClient[clientSocket].SetNickname(nickname);
		_MClient[clientSocket].SetUsername(username);
		_MClient[clientSocket].SetServername("");
		_MClient[clientSocket].SetMode("online");
		_MClient[clientSocket].setFullId(nickname + "!" + username + "@HOST");

		std::string reply = ":127.0.0.1 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!" + username + "@HOST\r\n";
		send(clientSocket, reply.c_str(), reply.size(), 0);
		reply = ":127.0.0.1 002 " + nickname + " :Your host is ourIRC, running version 1.0\r\n";
		send(clientSocket, reply.c_str(), reply.size(), 0);
		reply = ":127.0.0.1 003 " + nickname + " :This server was created not a long time ago\r\n";
		send(clientSocket, reply.c_str(), reply.size(), 0);
		reply = ":127.0.0.1 004 ourIRC 1.0\r\n";
		send(clientSocket, reply.c_str(), reply.size(), 0);
		reply = ":127.0.0.1 005 " + nickname + " PREFIX=(ov)@+ CHANTYPES=# CHARSET=UTF-8 :are supported by this server\r\n";
		send(clientSocket, reply.c_str(), reply.size(), 0);
		reply = ":127.0.0.1 376 " + nickname + " :End of /MOTD command.\r\n";
		send(clientSocket, reply.c_str(), reply.size(), 0);
		return;
	}
	else
	{
		std::string reply = ":127.0.0.1 464 " + nickname + " :Password incorrect\r\n";
		send(clientSocket, reply.c_str(), reply.size(), 0);
		return ;
	}
}

bool	Server::is_good_infos(std::string message, int clientSocket)
{
	std::cout << "Message = " << message << std::endl;
	if (message.find("PASS") == std::string::npos) {
		std::string reply = ":127.0.0.1 461 * PASS :No password supplied\r\n";
		send(clientSocket, reply.c_str(), reply.size(), 0);
		return (false);
	}
	if (message.find("NICK") == std::string::npos) {
		std::string reply = ": 461 * NICK :No nickname given\r\n";
		send(clientSocket, reply.c_str(), reply.size(), 0);
		return (false);
	}
	if (message.find("USER") == std::string::npos) {
		std::string reply = ": 461 * USER :Not enough parameters\r\n";
		send(clientSocket, reply.c_str(), reply.size(), 0);
		return (false);
	}
	return (true);
}



// write handleFirstConnection to handle the first connection of a new IRC client
// int Server::handleFirstConnection(int clientSocket)
// {
//	 //recv data give at first connection and answer knowing that the server need a password
//	 char buffer[BUFFER_SIZE];
//	 memset(buffer, 0, BUFFER_SIZE);
//	 int bytes_read = recv(clientSocket, buffer, sizeof(buffer), 0);
//	 // handle recv error
//	 if (bytes_read == -1) {
//		 std::cerr << "Error while receiving data from client -1" << std::endl;
//		 removeClient(clientSocket);
//		 return (0);
//	 }
//	 // handle client disconnection
//	 else if (bytes_read == 0)
//	 {
//		 std::cerr << "Client disconnected" << std::endl;
//		 removeClient(clientSocket);
//		 return (0);
//	 }
//	 std::string message(buffer);
//	 std::cout << "[first connexion] Message reçu : " << message << std::endl;
//	 if (message.find("PASS") == std::string::npos) {
//		 std::string reply = ":127.0.0.1 461 * PASS :No password supplied\r\n";
//		 send(clientSocket, reply.c_str(), reply.size(), 0);
//		 return (0);
//	 }
//	 if (message.find("NICK") == std::string::npos) {
//		 std::string reply = ": 461 * NICK :No nickname given\r\n";
//		 send(clientSocket, reply.c_str(), reply.size(), 0);
//		 return (0);
//	 }
//	 if (message.find("USER") == std::string::npos) {
//		 std::string reply = ": 461 * USER :Not enough parameters\r\n";
//		 send(clientSocket, reply.c_str(), reply.size(), 0);
//		 return (0);
//	 }
//	 // if all is good, we can add the client to the map
//	 std::string::size_type pass_pos = message.find("PASS ");
//	 std::string::size_type nick_pos = message.find("NICK ");
//	 std::string::size_type user_pos = message.find("USER ");
//	 std::string password = message.substr(pass_pos + 5, nick_pos - pass_pos - 6);
//	 std::string nickname = message.substr(nick_pos + 5, user_pos - nick_pos - 6);
//	 std::string username = message.substr(user_pos + 5, message.size() - user_pos - 6);
//	 password.erase(std::remove(password.begin(), password.end(), '\r'), password.end());
//	 password.erase(std::remove(password.begin(), password.end(), '\n'), password.end());
//	 nickname.erase(std::remove(nickname.begin(), nickname.end(), '\r'), nickname.end());
//	 nickname.erase(std::remove(nickname.begin(), nickname.end(), '\n'), nickname.end());
//	 username.erase(std::remove(username.begin(), username.end(), '\r'), username.end());
//	 username.erase(std::remove(username.begin(), username.end(), '\n'), username.end());
//	 username = username.substr(0, username.find(" "));
//	 if (password == _password)
//	 {
//		 for (std::map<int, Client>::iterator it = _MClient.begin(); it != _MClient.end(); ++it)
//		 {
//			 if (it->second.GetNickname() == nickname)
//			 {
//				 std::string reply = ":127.0.0.1 433 " + nickname + " :Nickname is already in use\r\n";
//				 send(clientSocket, reply.c_str(), reply.size(), 0);
//				 return (0);
//			 }
//		 }
//		 //inserer le client dans la map
//		 _MClient.insert(std::make_pair(clientSocket, Client()));
//		 _MClient[clientSocket].SetNickname(nickname);
//		 _MClient[clientSocket].SetUsername(username);
//		 _MClient[clientSocket].SetServername("");
//		 _MClient[clientSocket].SetMode("online");
//		 std::cout << "Client " << nickname << " Username : " << username << " connected" << std::endl;
//		 std::string reply = ":127.0.0.1 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!" + username + "@HOST\r\n";
//		 send(clientSocket, reply.c_str(), reply.size(), 0);
//		 reply = ":127.0.0.1 002 " + nickname + " :Your host is ourIRC, running version 1.0\r\n";
//		 send(clientSocket, reply.c_str(), reply.size(), 0);
//		 reply = ":127.0.0.1 003 " + nickname + " :This server was created not a long time ago\r\n";
//		 send(clientSocket, reply.c_str(), reply.size(), 0);
//		 reply = ":127.0.0.1 004 ourIRC 1.0";
//		 send(clientSocket, reply.c_str(), reply.size(), 0);
//		 reply = ":127.0.0.1 005 " + nickname + " PREFIX=(ov)@+ CHANTYPES=# CHARSET=UTF-8 :are supported by this server\r\n";
//		 send(clientSocket, reply.c_str(), reply.size(), 0);
//		 reply = ":127.0.0.1 376 " + nickname + " :End of /MOTD command.\r\n";
//		 send(clientSocket, reply.c_str(), reply.size(), 0);
//		 std::cout << "Sending to client" << std::endl;
//		 return (1);
//	 }
//	 else
//	 {
//		 std::string reply = ":127.0.0.1 464 " + nickname + " :Password incorrect\r\n";
//		 send(clientSocket, reply.c_str(), reply.size(), 0);
//		 return (0);
//	 }
// }

// void Server::handleFirstConnection(int clientSocket)
// {
//	 //recv NICK and PASS and then reply with good format
// 	std::cout << "[handleconnection] :" << clientSocket << "|" << std::endl;
//	 char buffer[BUFFER_SIZE];
//	 std::string nickname;
//	 std::string password;
//	 std::string username;
//	 memset(buffer, 0, BUFFER_SIZE);
//	 // int bytes_read = recv(clientSocket, buffer, sizeof(buffer), 0);
//	 // std::cout << "bytes_read : " << bytes_read << std::endl;
// 	recv(clientSocket, buffer, sizeof(buffer), 0);
//	 // if (bytes_read <= 0)
//	 // {
//	 //	 // Le client s'est déconnecté, supprimer son descripteur de fichier
//	 //	 std::cout << "Client disconnected by recv" << std::endl;
//	 //	 removeClient(clientSocket);
//	 // }
//	 // else
//	 // {
//		 // Traiter le message reçu
//		 std::string message(buffer);
//		 std::cout << "Message reçu : " << message << std::endl;
//		 if (message.find("NICK") != std::string::npos && message.find("PASS") != std::string::npos && message.find("USER") != std::string::npos)
//		 {
//			 std::string::size_type pass_pos = message.find("PASS ");
//			 std::string::size_type nick_pos = message.find("NICK ");
//			 std::string::size_type user_pos = message.find("USER ");
//			 std::string password = message.substr(pass_pos + 5, nick_pos - pass_pos - 6);
//			 std::string nickname = message.substr(nick_pos + 5, user_pos - nick_pos - 6);
//			 std::string username = message.substr(user_pos + 5, message.size() - user_pos - 6);
//			 if (password == _password)
//			 {
//				 std::string reply = ":127.0.0.1 001 " + nickname + " :Welcome to the Internet Relay Network " + nickname + "!" + username + "@HOST";
//				 send(clientSocket, reply.c_str(), reply.size(), 0);
//				 for (std::map<int, Client>::iterator it = _MClient.begin(); it != _MClient.end(); ++it)
//				 {
//					 if (it->second.GetNickname() == nickname)
//					 {
//						 std::string reply = ":127.0.0.1 433 " + nickname + " :Nickname is already in use";
//						 send(clientSocket, reply.c_str(), reply.size(), 0);
//						 return ;
//					 }
//				 }
//				 //inserer le client dans la map
//				 _MClient.insert(std::make_pair(clientSocket, Client()));
//				 _MClient[clientSocket].SetNickname(nickname);
//				 _MClient[clientSocket].SetUsername(username);
//				 _MClient[clientSocket].SetServername("");
//				 _MClient[clientSocket].SetMode("online");
//			 }
//			 else
//			 {
//				 std::string reply = ":127.0.0.1 464 " + nickname + " :Password incorrect";
//				 send(clientSocket, reply.c_str(), reply.size(), 0);
//			 }
//		 }
//		 else
//		 {
//			 std::string reply = ":127.0.0.1 464 " + nickname + " :Password incorrect";
//			 send(clientSocket, reply.c_str(), reply.size(), 0);
//		 }
//	 // }
// }
