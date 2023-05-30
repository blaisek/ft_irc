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

Client::Client(int fd) : _host(HOSTNAME)
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

