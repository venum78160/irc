#include <string>
#include <vector>


class Client {
public:
    Client(int sock_fd, const std::string& nickname, const std::string& username, const std::string& realname,
           const std::string& hostname, const std::string& servername, const std::string& mode)
        : sock_fd_(sock_fd), nickname_(nickname), username_(username), realname_(realname),
          hostname_(hostname), servername_(servername), mode_(mode) {}
    ~Client() {}

    // Accesseurs
    int GetSocketFD() const { return sock_fd_; }
    std::string GetNickname() const { return nickname_; }
    std::string GetUsername() const { return username_; }
    std::string GetRealname() const { return realname_; }
    std::string GetHostname() const { return hostname_; }
    std::string GetServername() const { return servername_; }
    std::string GetMode() const { return mode_; }
    std::vector<std::string> GetChannels() const { return channels_; }

    // Mutateurs
    void SetNickname(const std::string& nickname) { nickname_ = nickname; }
    void SetUsername(const std::string& username) { username_ = username; }
    void SetRealname(const std::string& realname) { realname_ = realname; }
    void SetHostname(const std::string& hostname) { hostname_ = hostname; }
    void SetServername(const std::string& servername) { servername_ = servername; }
    void SetMode(const std::string& mode) { mode_ = mode; }
    void AddChannel(const std::string& channel) { channels_.push_back(channel); }
    void RemoveChannel(const std::string& channel);

private:
    int sock_fd_;
	std::string nickname_;
    std::string username_;
    std::string realname_;
    std::string hostname_;
    std::string servername_;
    std::string mode_;
    std::vector<std::string> channels_;
};

// sock_fd_ : un entier qui représente le descripteur de fichier de la socket associée au client. Ce descripteur est utilisé par le serveur pour envoyer des données au client ou pour recevoir des données du client.
// nickname_ : une chaîne de caractères qui contient le surnom du client.
// username_ : une chaîne de caractères qui contient le nom d'utilisateur du client.
// realname_ : une chaîne de caractères qui contient le vrai nom du client.
// hostname_ : une chaîne de caractères qui contient l'adresse IP ou le nom d'hôte du client.
// servername_ : une chaîne de caractères qui contient le nom du serveur auquel le client est connecté.
// mode_ : une chaîne de caractères qui contient le mode du client (par exemple, s'il est en mode "invisible", "away", etc.).
// channels_ : un vecteur de chaînes de caractères qui contient les noms des canaux auxquels le client est actuellement connecté.
