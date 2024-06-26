#ifndef SERVER_HPP
#define SERVER_HPP

#include "headers.hpp"

#define MAX 42
#define BUFFER_SIZE 128

class	Client;
class	Channel;

class Server
{
	private:
		std::string						password;
		int								port;
		int								socketFd;
		struct sockaddr_in				sockAddr;
		struct pollfd					fds[MAX];
		std::map<int, Client*>			clients;
		std::vector<Channel*>			channels;
	public:
		Server(int port, std::string password);
		void	run();
		void	newClient();
		void	sender(int fd, std::string msg);
		void	handler(Client *client);
		void	remover(Client *client);
		~Server();
};

#endif