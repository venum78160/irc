#include "ft_irc.hpp"

Client::Client()
{
    buffer_.resize(BUFFER_SIZE);
	// memset(buffer_, 0, BUFFER_SIZE);
}

Client::Client(int sock_fd, const std::string& nickname, const std::string& username, const std::string& servername, const std::string& mode)
    : sock_fd_(sock_fd), nickname_(nickname), username_(username), servername_(servername), mode_(mode) 
	{
		buffer_.resize(BUFFER_SIZE);
	}

Client& Client::operator=(const Client& other)
{
    if (this != &other) {
        sock_fd_ = other.sock_fd_;
        nickname_ = other.nickname_;
        username_ = other.username_;
        servername_ = other.servername_;
        mode_ = other.mode_;
    }
    return *this;
}

bool Client::operator==(const Client& other) const
{
    return sock_fd_ == other.sock_fd_;
}

bool Client::operator<(const Client& other) const
{
    return sock_fd_ < other.sock_fd_;
}

Client::~Client() {}

int Client::GetSocketFD() const {
    return sock_fd_;
}

char* Client::GetBuffer()
{
	buffer_.push_back('\0');
    return buffer_.data();
}

std::string Client::GetNickname() const {
    return nickname_;
}

std::string Client::GetUsername() const {
    return username_;
}

std::string Client::GetServername() const {
    return servername_;
}

std::string Client::GetMode() const {
    return mode_;
}

std::vector<std::string> Client::GetChannels() const {
    return channels_;
}

void Client::SetSock_fd(int fd) {
    sock_fd_ = fd;
}

void Client::SetBuffer(const std::string& message) {
    buffer_.resize(message.size());
    std::copy(message.begin(), message.end(), buffer_.begin());
}

void Client::SetNickname(const std::string& nickname) {
    nickname_ = nickname;
}

void Client::SetUsername(const std::string& username) {
    username_ = username;
}

void Client::SetServername(const std::string& servername) {
    servername_ = servername;
}

void Client::SetMode(const std::string& mode) {
    mode_ = mode;
}
void Client::AddChannel(const std::string& channel) {
    channels_.push_back(channel);
}

void Client::RemoveChannel(const std::string& channel) {
    for (std::vector<std::string>::iterator it = channels_.begin(); it != channels_.end(); ++it) {

        if (*it == channel) {
            channels_.erase(it);
            break;
        }
    }
}

std::ostream &operator<<(std::ostream &out, const Client &client)
{
    out << client.GetNickname();
    return out;
}
