/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:57:52 by saeby             #+#    #+#             */
/*   Updated: 2023/05/30 14:17:38 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Client.hpp"

Client::Client(int fd, std::string host) : _host(host)
{
	this->_client_fd = fd;
	this->_authenticated = false;
	this->_is_operator = false;
}

// Default constructor
Client::Client(void)
{
	std::cout << "Default constructor called" << std::endl;
	return ;
}

// Copy constructor
Client::Client(const Client &other)
{
	(void) other;
	std::cout << "Copy constructor called" << std::endl;
	return ;
}

// Assignment operator overload
Client &Client::operator=(const Client &other)
{
	(void) other;
	std::cout << "Assignment operator called" << std::endl;
	return (*this);
}

// Destructor
Client::~Client(void)
{
	std::cout << "Destructor called" << std::endl;
	return ;
}

void Client::setNickName(std::string nick_name)
{
    this->_nick_name = nick_name;
}

void Client::setUserName(std::string user_name)
{
    this->_user_name = user_name;
}

void Client::setRealName(std::string real_name)
{
    this->_real_name = real_name;
}

std::string Client::getNickName(void) const
{
    return (this->_nick_name);
}

std::string Client::getUserName(void) const
{
    return (this->_user_name);
}

std::string Client::getRealName(void) const
{
    return (this->_real_name);
}

void Client::insert(std::pair<int, Client *> client)
{
    this->_clients.insert(client);
}

std::string Client::getHost(void) const
{
    return (this->_host);
}

void Client::setHost(std::string host)
{
    this->_host = host;
}

int Client::getClientFd(void) const
{
    return (this->_client_fd);
}