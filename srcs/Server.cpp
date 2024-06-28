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

				if (this->fds[i].revents & (POLLHUP | POLLERR))
				{
					delete (this->clients[this->fds[i].fd]);
					close(this->fds[i].fd);
					this->fds[i].fd = -1;
				}	
				fds[i].revents = 0;
			}
		}
		signal(SIGINT, this->signalHandler);
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
			break;
		}
	}
}

void	Server::handler(Client *client)
{
	char						buff[BUFFER_SIZE];
	int							r_bytes;
	std::vector<std::string>	cmds;

	memset(buff, 0, sizeof(buff));
	r_bytes = recv(client->getFd(), buff, sizeof(buff), 0);
	if (r_bytes > 0)
	{
		std::stringstream	ss(buff);
		std::string			cmd;


		while (std::getline(ss, cmd, '\n'))
		{
			if (cmd[cmd.size() - 1] == '\r')
				cmd.erase(cmd.size() - 1);

			std::vector<std::string>	tokens;
			this->parser(&tokens, cmd);
			
			std::cout << "Fd[" << client->getFd() << "]: " << cmd << std::endl; //for Server console to track

			if (tokens[0] == "NICK")
			{
				this->nick(client, tokens);
			}
			else if (tokens[0] == "USER")
			{
				this->user(client, tokens);
			}
			else if (tokens[0] == "PASS")
			{
				this->pass(client, tokens);
			}
			else if (tokens[0] == "HELP")
			{
				this->help(client, tokens);
			}
			else if (client->getIsRegistered() == false)
			{
				this->sender(client->getFd(), "Server Register first use command HELP to get help\r\n");
			}
			else if (tokens[0] == "JOIN")
			{
				this->join(client, tokens);
			}
			else if (tokens[0] == "PRIVMSG")
			{
				this->privmsg(client, tokens);
			}
			else if (tokens[0] == "PART")
			{
				this->part(client, tokens);
			}
			else if (tokens[0] == "NOTICE")
			{
				this->notice(client, tokens);
			}
			else if (tokens[0] == "QUIT")
			{
				this->quit(client, tokens);
			}
			else if (tokens[0] == "KICK")
			{
				this->kick(client, tokens);
			}
			else if (tokens[0] == "TOPIC")
			{
				this->topic(client, tokens);
			}
			else
			{
				this->sender(client->getFd(), "Server Unkown command use HELP for help\r\n");
			}
		}
	}
	else
	{
		perror("Client could is not be recived!");
	}
}

void	Server::nick(Client *client, std::vector<std::string> tokens)
{
	if (tokens.size() == 1)
	{
		this->sendError(client->getFd(), ERR_NONICKNAMEGIVEN, tokens);
	}
	else if (tokens.size() == 2)
	{
		if (this->isClientExist(tokens[1]) == false)
		{
			client->setNickName(tokens[1]);
			this->sender(client->getFd(), "Server :Nick name is setted!\r\n");
			this->checkRegister(client);
		}
		else
		{
			this->sendError(client->getFd(), ERR_NICKNAMEINUSE, tokens);
		}
	}
	else
	{
		this->sendError(client->getFd(), ERR_NEEDMOREPARAMS, tokens);
	}
}

void	Server::user(Client *client, std::vector<std::string> tokens)
{
	if (tokens.size() == 5)
	{
		client->setUserName(tokens[1]);
		client->setHostName(tokens[2]);
		client->setServerName(tokens[3]);
		client->setRealName(tokens[4]);
		this->sender(client->getFd(), "Server :User name is setted!\r\n");
		this->checkRegister(client);
	}
	else
	{
		this->sender(client->getFd(), "server :invalid parameter counts!\r\n");
	}
}

void	Server::pass(Client *client, std::vector<std::string> tokens)
{
	if (client->getIsPassed() == true)
	{
		this->sendError(client->getFd(), ERR_ALREADYREGISTERED, tokens);
	}
	else
	{
		if (tokens.size() == 2)
		{
			if (tokens[1] == this->password)
			{
				client->setIsPassed(true);
				this->checkRegister(client);
			}
			else
			{
				this->sendError(client->getFd(), ERR_PASSWDMISMATCH, tokens);
			}
		}
		else
		{
			this->sendError(client->getFd(), ERR_NEEDMOREPARAMS, tokens);
		}
	}
}

void	Server::help(Client *client, std::vector<std::string> tokens)
{
	if (client->getIsRegistered() == false)
	{
		this->sender(client->getFd(), "Server :To set your nick name use Nick <nickName>!\r\n");
		this->sender(client->getFd(), "Server :To set your user name use NAME <userName> <hostName> <serverName> :<realName>!\r\n");
		this->sender(client->getFd(), "Server :To enter the password use PASS <password>!\r\n");
	}
	else
	{
		this->sender(client->getFd(), "Server :At command explanation <values> are values without < and >!\r\n");
		this->sender(client->getFd(), "Server :At command explanation [values] are optional!\r\n");
		this->sender(client->getFd(), "Server :Syntax : is used for last paramaters not divide words by space and take as a one parameter!\r\n");

		this->sender(client->getFd(), "Server :-------------------------------------------------------------:\r\n");

		this->sender(client->getFd(), "Server :To massage someone with privately use PRIVMSG <nickName> :<msg>!\r\n");
		this->sender(client->getFd(), "Server :To leave a channel use PART <channelName> [:<msg>]!\r\n");
		this->sender(client->getFd(), "Server :To leave a channel use NOTICE <channelName> :<msg>\r\n");
		this->sender(client->getFd(), "Server :To leave the server use QUIT\r\n");
	}
}

void	Server::join(Client *client, std::vector<std::string> tokens)
{
	if (tokens.size() == 2)
	{
		if (this->channels[tokens[1]] == NULL)
		{
			this->channels[tokens[1]] = new Channel(tokens[1], client);
			this->sender(client->getFd(), "Server :Channel has been created!\r\n");
		}
		else
		{
			this->channels[tokens[1]]->addClient(client);
			this->sender(client->getFd(), "Server :You joined the channel!\r\n");
		}
	}
	else
	{
		this->sendError(client->getFd(), ERR_NEEDMOREPARAMS, tokens);
	}
}

void	Server::privmsg(Client *client, std::vector<std::string> tokens)
{
	bool	flag = false;

	if (tokens.size() == 3)
	{
		for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
		{
			if (it->second->getNickName() == tokens[1])
			{
				tokens[2] = client->getNickName() + " :" + tokens[2] + "\r\n";
				this->sender(it->second->getFd(), tokens[2]);
				flag = true;
				break;
			}
		}
		if (flag == true)
		{
			this->sender(client->getFd(), "Server :Message send!\r\n");
		}
		else
		{
			this->sender(client->getFd(), "Server :Client did not found!\r\n");
		}
	}
	else
	{
		this->sender(client->getFd(), "Server :invalid parameter counts!\r\n");
	}
}

void	Server::part(Client *client, std::vector<std::string> tokens)
{
	if (tokens.size() == 2 && tokens.size() == 3)
	{
		Channel *channel = this->channels[tokens[1]];

		if (channel != NULL)
		{
			channel->removeClient(client);
			if (tokens.size() == 3)
			{
				tokens[2] = client->getNickName() + " :" + tokens[2] + "\r\n";
				channel->channelSender(tokens[2]);
			}
		}
		else
		{
			this->sender(client->getFd(), "Server :Channel did not found!\r\n");
		}
	}
	else
	{
		this->sender(client->getFd(), "Server :invalid parameter counts!\r\n");
	}
}

void	Server::notice(Client *client, std::vector<std::string> tokens)
{
	if (tokens.size() == 3)
	{
		Channel *channel = this->channels[tokens[1]];

		if (channel != NULL)
		{
			tokens[2] = client->getNickName() + " :" + tokens[2] + "\r\n";
			channel->channelSender(tokens[2]);
		}
		else
		{
			this->sender(client->getFd(), "Server :Channel did not found!\r\n");
		}
	}
	else
	{
		this->sender(client->getFd(), "Server :invalid parameter counts!\r\n");
	}

}

void	Server::quit(Client *client, std::vector<std::string> tokens)
{
	(void) tokens;

	for (int i = 1; i < MAX; i++)
	{
		if (this->fds[i].fd == client->getFd())
		{
			this->fds[i].fd = -1;
			close(client->getFd());
			delete (client);
		}
	}
}

void	Server::kick(Client *client, std::vector<std::string> tokens)
{

}

void	Server::topic(Client *client, std::vector<std::string> tokens)
{

}

void	Server::signalHandler(int sigNum)
{
	std::cout << sigNum << std::endl;
	exit (1);
}

void	Server::sender(int fd, std::string msg)
{
	send(fd, msg.c_str(), msg.size(), 0);
}

bool	Server::isClientExist(std::string nickName)
{
	for (std::map<int, Client*>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
	{
		if (nickName == it->second->getNickName())
		{
			return (true);
		}
	}
	return (false);
}

void	Server::checkRegister(Client *client)
{
	if (client->getIsRegistered() == false)
	{
		if (client->getIsPassed() != false && client->getNickName() != "" && client->getUserName() != "")
		{
			client->setIsRegistered(true);
			this->sender(client->getFd(), "Server :You succesfully registered!\r\n");
		}
	}
}

void	Server::parser(std::vector<std::string> *tokens, std::string cmd)
{
	size_t		pos = cmd.find(':');
	std::string	r_str = "";
	std::string	l_str = "";

	if (pos != std::string::npos)
	{
		l_str = cmd.substr(0, pos - 1);
		r_str = cmd.substr(pos, cmd.size());

		if (r_str[0] == ':')
			r_str.erase(0, 1);
		while (r_str[0] == ' ')
			r_str.erase(0, 1);
	}
	else
	{
		l_str = cmd;
	}

	std::stringstream			line(l_str);
	std::string					str;

	while (std::getline(line, str, ' '))
	{
		tokens->push_back(str);
	}

	if (r_str != "")
	{
		tokens->push_back(r_str);
	}
}

void	Server::sendError(int fd, int code, std::vector<std::string> tokens)
{
	if (code == ERR_NEEDMOREPARAMS)
	{
		std::string msg = ":" + tokens[0] + " " + std::to_string(code) + " :Not enough parameters\r\n";
		this->sender(fd, msg);
	}
	else if (code == ERR_ALREADYREGISTERED)
	{
		std::string msg = ":" + tokens[0] + " " + std::to_string(code) + " :You are already registered\r\n";
		this->sender(fd, msg);
	}
	else if (code == ERR_PASSWDMISMATCH)
	{
		std::string msg = +i std::to_string(code) + ":Password incorrect\r\n";
		this->sender(fd, msg);
	}
	else if (code == ERR_NONICKNAMEGIVEN)
	{
		std::string msg = ":" + tokens[0] + " " + std::to_string(code) + " :No nickname given\r\n";
		this->sender(fd, msg);
	}
	else if (code == ERR_NICKNAMEINUSE)
	{
		std::string msg = ":" + tokens[0] + " " + std::to_string(code) + " :" + tokens[1] + " :Nickname is already in use\r\n";
		this->sender(fd, msg);
	}
}

Server::~Server()
{

}
