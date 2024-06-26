#include "../incs/Channel.hpp"

Channel::Channel(std::string name, Client *admin)
{
	this->name = name;
	this->admin = admin;
	this->addClient(admin);
	this->addOperator(admin);
}

void				Channel::setName(std::string name)
{
	this->name = name;
}

const std::string	Channel::getName() const
{
	return (this->name);
}

void				Channel::setTopic(std::string topic)
{
	this->topic = topic;
}

const std::string	Channel::getTopic() const
{
	return (this->topic);
}

void				Channel::addClient(Client *client)
{
	this->clients.push_back(client);
}

void				Channel::removeClient(Client *client)
{
	std::vector<Client *>::iterator it = std::find(this->clients.begin(), this->clients.end(), client);

	if (it != this->clients.end())
		this->clients.erase(it);
}

void				Channel::addOperator(Client *client)
{
	this->operators.push_back(client);
}

void				Channel::removeOperator(Client *client)
{
	std::vector<Client *>::iterator it = std::find(this->operators.begin(), this->operators.end(), client);

	if (it != this->operators.end())
		this->operators.erase(it);
}

Channel::~Channel()
{

}
