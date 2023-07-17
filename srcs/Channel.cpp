/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 15:50:51 by btchiman          #+#    #+#             */
/*   Updated: 2023/07/17 22:07:12 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(void)
{
    return ;
}

Channel::Channel(std::string name) : _name(name), _password(""), _limit(0)
{
	// |o|p|s|i|t|n|b|v
	this->_modes.insert(std::make_pair('o', false));
	this->_modes.insert(std::make_pair('p', false));
	this->_modes.insert(std::make_pair('s', false));
	this->_modes.insert(std::make_pair('i', false));
	this->_modes.insert(std::make_pair('t', false));
	this->_modes.insert(std::make_pair('n', false));
	this->_modes.insert(std::make_pair('b', false));
	this->_modes.insert(std::make_pair('v', false));
	this->_modes.insert(std::make_pair('k', false));
    // initialiser d'autres attributs du canal, configurer des paramètres, etc.
    return ;
}

Channel::Channel(const Channel& other)
{
    *this = other;
    return ;
}

Channel &Channel::operator=(const Channel &other)
{
    this->_name = other._name;
    this->_clients = other._clients;
    return (*this);
}

Channel::~Channel()
{

    return ;
}

std::string Channel::getName(void) const
{
    return (this->_name);
}

void Channel::setName(std::string name)
{
    this->_name = name;
}

void Channel::addClient(Client *client)
{
    this->_clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
    std::vector<Client *>::iterator it;

    it = std::find(this->_clients.begin(), this->_clients.end(), client);
    if (it != this->_clients.end())
        this->_clients.erase(it);
}

std::vector<Client *> Channel::getClients(void) const
{
    return (this->_clients);
}

Client*	Channel::getClient(std::string nick) const
{
	std::vector<Client *>::const_iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if ((*it)->getNick() == nick)
			return (*it);
	}
	return (nullptr);
}

void Channel::setPassword(std::string password)
{
    this->_password = password;
}

std::string Channel::getPassword(void) const
{
    return (this->_password);
}

void Channel::addOperator(Client *client)
{
    this->_operators.push_back(client);
}

std::vector<Client *> Channel::getOperators(void) const
{
    return (this->_operators);
}

void Channel::removeOperator(Client *client)
{
    std::vector<Client *>::iterator it;

    it = std::find(this->_operators.begin(), this->_operators.end(), client);
    if (it != this->_operators.end())
        this->_operators.erase(it);
}

void Channel::addBanned(int fd)
{
    this->_banned.push_back(fd);
}

std::vector<int> Channel::getBanned(void) const
{
    return (this->_banned);
}

void Channel::removeBanned(int fd)
{
    std::vector<int>::iterator it;

    it = std::find(this->_banned.begin(), this->_banned.end(), fd);
    if (it != this->_banned.end())
        this->_banned.erase(it);
}

bool Channel::hasNickname(std::string nickname) const
{
    std::vector<Client *>::const_iterator it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if ((*it)->getNick() == nickname)
            return (true);
    }
    return (false);
}

void Channel::addNickname(std::string nickname)
{
    this->_nicknames.push_back(nickname);
}

std::string Channel::getNicknamesList(void) const
{
    std::string nicknames;
    std::vector<Client *>::const_iterator it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
		if (this->isOp((*it)->getNick()))
			nicknames += "@";
        nicknames += (*it)->getNick();
        nicknames += " ";
    }
    return (nicknames);
}

void Channel::setTopic(std::string topic)
{
    this->_topic = topic;
}

bool Channel::hasPassword(void) const
{
    if (this->_password == "")
        return (false);
    return (true);
}

bool Channel::find(int fd) {
    std::vector<Client *>::iterator it;

    for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
        if ((*it)->getFd() == fd)
            return (true);
    }
    return (false);
}

bool Channel::empty() const {
    if (this->_clients.empty())
        return (true);
    return (false);
}


void	Channel::removeUser(std::string nick)
{
	this->_nicknames.erase(std::remove(this->_nicknames.begin(), this->_nicknames.end(), nick), this->_nicknames.end());
	
	for (unsigned int i = 0; i < this->_clients.size(); i++)
	{
		if (this->_clients[i]->getNick() == nick)
		{
			this->_clients.erase(this->_clients.begin() + i);
			break ;
		}
	}

	for (unsigned int i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i]->getNick() == nick)
		{
			this->_operators.erase(this->_operators.begin() + i);
			break ;
		}
	}
	this->removeNickname(nick);
}

void	Channel::setMode(char mode, bool setMode) { this->_modes[mode] = setMode; }

std::string	Channel::getModes(void) const
{
	std::string	modes;

	for (std::map<char, bool>::const_iterator it = this->_modes.begin(); it != this->_modes.end(); it++)
	{
		if (it->second)
			modes.append(std::string(1, it->first));
	}
	if (modes.length() > 0)
		modes.insert(modes.begin(), '+');
	modes.append("\r\n");
	return (modes);
}

bool	Channel::getMode(char mode) const
{
    for (std::map<char, bool>::const_iterator it = this->_modes.begin(); it != this->_modes.end(); it++)
    {
        if (it->first == mode)
            return (it->second);
    }
    return (false);
}

void	Channel::setLimit(int limit) { this->_limit = limit; }

int	Channel::getLimit(void) const { return (this->_limit); }

void	Channel::addInvited(std::string nick)
{
	this->_invited.push_back(nick);
}

void	Channel::removeInvited(std::string nick)
{
		this->_invited.erase(std::remove(this->_invited.begin(), this->_invited.end(), nick), this->_invited.end());
}

bool	Channel::isInvited(std::string nick) const
{
	if (std::find(this->_invited.begin(), this->_invited.end(), nick) != this->_invited.end())
		return (true);
	return (false);
}

std::vector<std::string>	Channel::getInvited(void) const
{
	return (this->_invited);
}

bool	Channel::isOp(std::string nick) const
{
	for (unsigned int i = 0; i < this->_operators.size(); i++)
	{
		if (this->_operators[i]->getNick() == nick)
			return (true);
	}
	return (false);
}

void Channel::removeNickname(std::string nickname) {
    if(!this->_nicknames.empty()) {
        std::vector<std::string>::iterator it;
        it = std::find(this->_nicknames.begin(), this->_nicknames.end(), nickname);
        if (it != this->_nicknames.end())
            this->_nicknames.erase(it);
    }
}

std::string Channel::getTopic() const {
    return (this->_topic);
}

std::vector<std::string>	Channel::getNickNames(void) const
{
	return (this->_nicknames);
}