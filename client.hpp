#pragma once
#include "ft_irc.hpp"

// sock_fd_ : un entier qui représente le descripteur de fichier de la socket associée au client. Ce descripteur est utilisé par le serveur pour envoyer des données au client ou pour recevoir des données du client.
// nickname_ : une chaîne de caractères qui contient le surnom du client.
// username_ : une chaîne de caractères qui contient le nom d'utilisateur du client.
// realname_ : une chaîne de caractères qui contient le vrai nom du client.
// hostname_ : une chaîne de caractères qui contient l'adresse IP ou le nom d'hôte du client.
// servername_ : une chaîne de caractères qui contient le nom du serveur auquel le client est connecté.
// mode_ : une chaîne de caractères qui contient le mode du client (par exemple, s'il est en mode "invisible", "away", etc.).
// channels_ : un vecteur de chaînes de caractères qui contient les noms des canaux auxquels le client est actuellement connecté.

class Client {
public:
    Client();
    Client(int sock_fd, const std::string& nickname, const std::string& username, const std::string& servername, const std::string& mode);
	Client& operator=(const Client& other);
    ~Client();
    bool operator<(const Client& other) const;
    bool operator==(const Client& other) const;

    // Accesseurs
    int GetSocketFD() const;
	char	*GetBuffer();
    std::string GetNickname() const;
    std::string GetUsername() const;
    std::string GetServername() const;
    std::string getFullId() const;
    std::string GetMode() const;
    std::vector<std::string> GetChannels() const;

    // Mutateurs
	void SetSock_fd(int fd);
	void SetBuffer(const std::string& message);
    void SetNickname(const std::string& nickname);
    void SetUsername(const std::string& username);
    void SetServername(const std::string& servername);
    void setFullId( const std::string &fullId );
    void SetMode(const std::string& mode);
    void AddChannel(const std::string& channel);
    void RemoveChannel(const std::string& channel);
	void printClientInfo();

private:
	std::vector<char> buffer_;
    int sock_fd_;
    std::string nickname_;
    std::string username_;
    std::string servername_;
    std::string mode_;
    std::string fullId_;
    std::vector<std::string> channels_;
};

std::ostream &operator<<(std::ostream &out, const Client &client);
