#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "headers.hpp"

class Client;

class Channel
{
	private:
		std::string				name;
		std::string				topic;

		int						key;
		Client					*admin;

		std::vector<Client *>	clients;
		std::vector<Client *>	operators;
	public:
		Channel(std::string name, Client *admin);

		void				setName(std::string name);
		const std::string	getName() const;

		void				setTopic(std::string topic);
		const std::string	getTopic() const;

		void				addClient(Client *client);
		void				removeClient(Client *client);

		void				addOperator(Client *client);
		void				removeOperator(Client *client);

		void				channelSender(std::string msg);
		~Channel();
};

#endif
