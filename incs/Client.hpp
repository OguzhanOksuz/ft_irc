#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "headers.hpp"

class Channel;

class Client
{
	private:
		std::string		nickName;
		std::string		userName;
		std::string		serverName;
		std::string		realName;
		std::string		hostName;
		int				fd;
		bool			isRegistered;
		bool			isPassed;
	public:
		Client(int fd);

		const std::string	getNickName() const;
		const std::string	getUserName() const;
		const std::string	getRealName() const;
		const std::string	getHostName() const;
		const std::string	getServerName() const;

		int					getFd() const;

		bool				getIsRegistered() const;
		bool				getIsPassed() const;

		void	setNickName(const std::string nickName);
		void	setUserName(const std::string UserName);
		void	setRealName(const std::string RealName);
		void	setHostName(const std::string HostName);
		void	setServerName(const std::string serverName);

		void	setIsRegistered(bool IsRegistered);
		void	setIsPassed(bool IsPassed);
		~Client();
};

#endif
