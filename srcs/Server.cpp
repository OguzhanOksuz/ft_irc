#include "../incs/Server.hpp"

Server::Server(int port, std::string password)
{
	this->port = port;
	this->password = password;
	this->socketFd = -1;
	this->sockAddr.sin_family = AF_INET;
	this->sockAddr.sin_port = htons(this->port);
	this->sockAddr.sin_addr.s_addr = INADDR_ANY;


	this->socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socketFd == -1)
		throw std::runtime_error("Server socket could not initilized!");
	if (bind(this->socketFd, (struct sockaddr *)&(this->sockAddr), sizeof(this->sockAddr)) == -1)
		throw std::runtime_error("Server socket could not binded!");
	if (listen(this->socketFd, MAX) == -1)
		throw std::runtime_error("Server socket could not be listened!");

	this->fds[0].fd = this->socketFd;
	this->fds[0].events = POLLIN;
	for (int i = 1; i < MAX; i++)
		this->fds[i].fd = -1;

	std::cout << "Server has been initilized!" << std::endl;
	this->run();
}

void	Server::run()
{
	while (true)
	{
		if (poll(this->fds, MAX, -1) == -1)
			perror("Server could not be polled!");
		if (this->fds[0].revents & POLLIN)
			this->newClient();

		for (int i = 1; i < MAX; i++)
		{
			if (this->fds[i].fd != -1)
			{
				if (this->fds[i].revents & POLLIN)
				{
					this->handler(this->clients[this->fds[i].fd]);
				}
				else if(this->fds[i].revents & (POLLHUP | POLLERR))
				{
					this->remover(this->clients[this->fds[i].fd])
				}
				fds[i].revents = 0;
			}
		}
	}
}

void	Server::newClient()
{
			struct sockaddr_in	newClientAddr;
			int					newClient;

			newClient = accept(this->socketFd, reinterpret_cast<sockaddr*>(&newClientAddr), reinterpret_cast<socklen_t*>(&newClientAddr));
			if (newClient == -1)
				perror("Client could not be accepted!");
			for (int i = 1; i < MAX; i++)
			{
				if (this->fds[i].fd == -1)
				{	
					if (fcntl(newClient, F_SETFL, O_NONBLOCK) == -1)
						perror("Client fcntl is not accessable!");
					this->fds[i].fd = newClient;
					this->fds[i].events = POLLIN;
					this->clients[this->fds[i].fd] = new Client(this->fds[i].fd);
					sender(this->fds[i].fd, "SERVER You must authanticate!\r\n");
					sender(this->fds[i].fd, "SERVER For help use HELP command!\r\n");
					printf("Bağlandı\n");
					break;
				}
			}
}

void	Server::handler(Client *client)
{

}

void	Server::remover(Client *client)
{

}

void	Server::sender(int fd, std::string msg)
{
	send(fd, msg.c_str(), msg.size(), 0);
}


Server::~Server()
{

}
