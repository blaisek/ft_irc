/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 11:42:13 by saeby             #+#    #+#             */
/*   Updated: 2023/07/19 12:01:50 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string	Server::_cmd_oper(Request& req, int fd)
{
	(void) req;
	(void) fd;
	if (req.params.size() < 2)
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameters given.\r\n"));
	if (std::find(this->_nicknames.begin(), this->_nicknames.end(), req.params[0]) == this->_nicknames.end())
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOSUCHNICK, ":No such nick\r\n"));
	else
	{
		if (req.params[1] != this->_oper_pass)
			return (this->_get_message(this->_clients[fd]->getNick(), ERR_PASSWDMISMATCH, ":Wrong operator password.\r\n"));
		this->_operators.push_back(this->_clients[fd]->getNick());
		return (this->_get_message(this->_clients[fd]->getNick(), RPL_YOUREOPER, ":You're now an IRC operator.\r\n"));
	}
	return ("");
}