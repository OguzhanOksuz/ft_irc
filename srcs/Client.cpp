#include "../incs/Client.hpp"

Client::Client(int fd)
{
	this->nickName = "";
	this->userName = "";
	this->realName = "";
	this->hostName = "";
	this->serverName = "";
	this->fd = fd;
	this->isRegistered = false;
	this->isPassed = false;
}

const std::string Client::getNickName() const
{
	return (this->nickName);
}

const std::string Client::getUserName() const
{
	return (this->userName);
}

const std::string Client::getRealName() const
{
	return (this->realName);
}

const std::string Client::getHostName() const
{
	return (this->hostName);
}

const std::string Client::getServerName() const
{
	return (this->serverName);
}

int	Client::getFd() const
{
	return (this->fd);
}

bool	Client::getIsRegistered() const
{
	return (this->isRegistered);
}

bool	Client::getIsPassed() const
{
	return (this->isPassed);
}

void	Client::setNickName(const std::string nickName)
{
	this->nickName = nickName;
}

void	Client::setUserName(const std::string userName)
{
	this->userName = userName;
}

void	Client::setRealName(const std::string realName)
{
	this->realName = realName;
}

void	Client::setHostName(const std::string hostName)
{
	this->hostName = hostName;
}

void	Client::setServerName(const std::string serverName)
{
	this->serverName = serverName;
}

void	Client::setIsRegistered(bool isRegistered)
{
	this->isRegistered = isRegistered;
}

void	Client::setIsPassed(bool isPassed)
{
	this->isPassed = isPassed;
}

Client::~Client()
{
	std::cout << "Client " << this->nickName << ", left the server!" << std::cout;
}
