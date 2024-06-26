#include "../incs/Server.hpp"

int main(int ac, char **av)
{
	int port;

	if (ac == 3 && av[1][0])
	{
		for (int i = 0; av[1][i]; i++)
		{
			if (av[1][i] > '9' || av[1][i] < '0')
			{
				std::cout << "Port must be a number between 0 and 65535!" << std::endl;
				return (0);
			}
		}
		port = std::stoi(av[1]);
		if (port < 0 || port > 65535)
		{
			std::cout << "Port must be a number between 0 and 65535!" << std::endl;
		}
		else
		{
			Server(port, av[2]);
		}
	}
	else
	{
		std::cout << "Wrong arguman count!" << std::endl;
		std::cout << "./ircserv <port> <password>" << std::endl;
	}
	return (0);
}