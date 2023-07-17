/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 21:14:50 by saeby             #+#    #+#             */
/*   Updated: 2023/07/17 23:07:53 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string	Server::_cmd_who(Request& req, int fd)
{
	if (req.params.size() == 0)
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameters given.\r\n"));
	if (req.params.size() == 1)
	{
		// check if param is a channel name
		if (this->_channels.find(req.params[0]) != this->_channels.end())
		{
			// "<channel> <user> <host> <server> <nick> <H|G>[*][@|+] :<hopcount> <real name>"
			std::vector<std::string> nicks = this->_channels[req.params[0]]->getNickNames();
			std::string ret;
			for (unsigned int i = 0; i < nicks.size(); i++)
			{
				std::string tmp = ":"+this->_name + " " + RPL_WHOREPLY + " ";
				tmp.append(this->_clients[fd]->getNick() + " ");
				tmp.append(req.params[0] + " ~" + this->_clients[this->_fdByNick(nicks[i])]->getUser() + " ");
				tmp.append(this->_clients[this->_fdByNick(nicks[i])]->getIp() + " " + this->_name + " ");
				tmp.append(this->_clients[this->_fdByNick(nicks[i])]->getNick() + " ");
				tmp.append("H");
				if (this->_channels[req.params[0]]->isOp(nicks[i]))
					tmp.append("@");
				tmp.append(" :0 " + this->_clients[this->_fdByNick(nicks[i])]->getFullName());
				tmp.append("\r\n");
				ret.append(tmp);
			}
			ret.append(":" + this->_name + " " + RPL_ENDOFWHO + " " + this->_clients[fd]->getNick() + " " + req.params[0]);
			ret.append(" :End of /WHO list.\r\n");
			return (ret);
		}
		else
			return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOSUCHNICK, ":No such nick / channel\r\n"));
	}
	else if (req.params.size() == 2 && req.params[1] == "o")
	{
		// only return operators
		std::vector<std::string> nicks = this->_channels[req.params[0]]->getNickNames();
		std::string ret;
		for (unsigned int i = 0; i < nicks.size(); i++)
		{
			if (this->_channels[req.params[0]]->isOp(nicks[i]))
			{
				std::string tmp = ":"+this->_name + " " + RPL_WHOREPLY + " ";
				tmp.append(this->_clients[fd]->getNick() + " ");
				tmp.append(req.params[0] + " ~" + this->_clients[this->_fdByNick(nicks[i])]->getUser() + " ");
				tmp.append(this->_clients[this->_fdByNick(nicks[i])]->getIp() + " " + this->_name + " ");
				tmp.append(this->_clients[this->_fdByNick(nicks[i])]->getNick() + " ");
				tmp.append("H");
				tmp.append("@");
				tmp.append(" :0 " + this->_clients[this->_fdByNick(nicks[i])]->getFullName());
				tmp.append("\r\n");
				ret.append(tmp);
			}
		}
		ret.append(":" + this->_name + " " + RPL_ENDOFWHO + " " + this->_clients[fd]->getNick() + " " + req.params[0]);
		ret.append(" :End of /WHO list.\r\n");
		return (ret);
	}
	return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameters given.\r\n"));
}