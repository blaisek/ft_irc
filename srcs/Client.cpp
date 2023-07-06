/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 16:18:34 by saeby             #+#    #+#             */
/*   Updated: 2023/07/06 19:33:55 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

// Default constructor
Client::Client(void) : _fd(), _auth(false), _nick(""), _user(""), _identity(""), _host(HOSTNAME), _reg(false), _fullName("") , _srvOperator(false), _channels()
{
	this->_modes.insert(std::make_pair('i', false));
	this->_modes.insert(std::make_pair('s', false));
	this->_modes.insert(std::make_pair('w', false));
	this->_modes.insert(std::make_pair('o', false));
}

Client::Client(int fd) : _fd(fd), _auth(false) , _nick(""), _user(""), _identity(""), _host(HOSTNAME), _reg(false), _fullName(""), _srvOperator(false), _channels()
{
	this->_modes.insert(std::make_pair('i', false));
	this->_modes.insert(std::make_pair('s', false));
	this->_modes.insert(std::make_pair('w', false));
	this->_modes.insert(std::make_pair('o', false));
}

// Copy constructor
Client::Client(const Client &other)
{
	(void) other;
	return ;
}

// Assignment operator overload
Client &Client::operator=(const Client &other)
{
	(void) other;
	return (*this);
}

// Destructor
Client::~Client(void)
{
	return ;
}

int			Client::getFd(void) const { return (this->_fd); }
bool		Client::isAuth(void) const { return (this->_auth); }
std::string	Client::getNick(void) const { return (this->_nick); }
std::string	Client::getUser(void) const { return (this->_user); }
std::string	Client::getIdentity(void) const { return (this->_identity); }
std::string	Client::getHost(void) const { return (this->_host); }
bool		Client::getReg(void) const { return (this->_reg); }
std::string	Client::getFullName(void) const { return (this->_fullName); }
std::vector<std::string>	Client::getChans(void) { return (this->_channels); }
bool		Client::isOp(void) const { return (this->_srvOperator); }

std::string	Client::getModes(void) const
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

void		Client::setNick(std::string nick) { this->_nick = nick; }
void		Client::setUser(std::string user) { this->_user = user; }
void		Client::setIdentity(std::string identity) { this->_identity = identity; }
void		Client::setHost(std::string host) { this->_host = host; }
void		Client::setReg(bool reg) { this->_reg = reg; }
void		Client::setAuth(bool auth) { this->_auth = auth; }
void		Client::setFullName(std::string fullname) { this->_fullName = fullname; }

void		Client::setMode(char mode, bool setMode) { this->_modes[mode] = setMode; }

void		Client::join(std::string channelName)
{
	this->_channels.push_back(channelName);
}

void		Client::leave(std::string channelName)
{
	this->_channels.erase(std::remove(this->_channels.begin(), this->_channels.end(), channelName), this->_channels.end());
}

std::ostream &operator<<(std::ostream &o, const Client &c)
{
	o << "====================" << std::endl;
	o << "Client FD: " << c.getFd() << std::endl;
	o << "Authenticated: " << (c.isAuth() ? "true" : "false") << std::endl;
	o << "====================";
	return (o);
}
