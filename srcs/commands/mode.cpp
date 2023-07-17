/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/16 14:37:42 by saeby             #+#    #+#             */
/*   Updated: 2023/07/17 21:32:50 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string	Server::_cmd_mode(Request& req, int fd)
{
	if (!this->_clients[fd]->getReg())
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOTREGISTERED, ":You must be registered to do this.\r\n"));
	if (req.params.size() == 0)
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameters given.\r\n"));

	if (std::find(this->_nicknames.begin(), this->_nicknames.end(), req.params[0]) != this->_nicknames.end())
		return (this->_userMode(req, fd));
	else if (this->_channels.find(req.params[0]) != this->_channels.end())
		return (this->_channelMode(req, fd));
	else
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOSUCHNICK, ":No such nick / channel\r\n"));
}

std::string	Server::_userMode(Request& req, int fd)
{
	if (req.params.size() == 1)
	{
		if (req.params[0] == this->_clients[fd]->getNick())
			return (this->_get_message(this->_clients[fd]->getNick(), RPL_UMODEIS, this->_clients[fd]->getModes()));
		else
		{
			if (this->_clients[fd]->isOp())
				return (this->_clients[this->_fdByNick(req.params[0])]->getModes());
			return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOPRIVILEGES, ":You do not have rights to see other users' mode.\r\n"));
		}
	}
	else
	{
		bool	validMode = true;
		std::string	chdModes;
		if (req.params[1][0] != '-' && req.params[1][0] != '+')
			return (this->_get_message(this->_clients[fd]->getNick(), ERR_UNKNOWNMODE, std::string(1, req.params[1][0]) + " :is unknown mode char to me.\r\n"));
		std::vector<char>	modes = this->_splitModes(req.params[1]);
		chdModes.append(std::string(1, req.params[1][0]));
		char m = this->_validUserMode(modes, validMode);
		if (!validMode)
			return (this->_get_message(this->_clients[fd]->getNick(), ERR_UNKNOWNMODE, std::string(1, m) + " :is unknown mode char to me.\r\n"));
		for (unsigned int i = 0; i < modes.size(); i++)
		{
			bool setMode = req.params[1][0] == '+' ? true : false;
			if (setMode && modes[i] == 'o')
				continue ;
			chdModes.append(std::string(1, modes[i]));
			this->_clients[fd]->setMode(modes[i], setMode);
		}
		return (":" + this->_clients[fd]->getNick() + " MODE " + this->_clients[fd]->getNick() + " " + chdModes + "\r\n");
	}
}

std::string	Server::_channelMode(Request& req, int fd)
{
	(void) req;
	(void) fd;
	if (req.params.size() == 1)
	{
		// :*.freenode.net 324 saeby #freenode-bbs :+Pnrt
		std::string ret = ":";
		ret.append(this->_name);
		ret.append(" ");
		ret.append(RPL_CHANNELMODEIS);
		ret.append(" ");
		ret.append(this->_clients[fd]->getNick());
		ret.append(" ");
		ret.append(req.params[0]);
		ret.append(" :");
		ret.append(this->_channels[req.params[0]]->getModes());
		return (ret);
	}
	else
	{
		std::vector<Client *>	channelClients = this->_channels[req.params[0]]->getOperators();
		if (std::find(channelClients.begin(), channelClients.end(), this->_clients[fd]) == channelClients.end())
			return (this->_get_message(this->_clients[fd]->getNick(), ERR_CHANOPRIVSNEEDED, ":You need channel operators privileges.\r\n"));

		bool	validMode = true;
		std::string	chdModes;
		std::string	trailing = "";
		if (req.params[1][0] != '-' && req.params[1][0] != '+')
			return (this->_get_message(this->_clients[fd]->getNick(), ERR_UNKNOWNMODE, std::string(1, req.params[1][0]) + " :is unknown mode char to me.\r\n"));
		std::vector<char>	modes = this->_splitModes(req.params[1]);
		chdModes.append(std::string(1, req.params[1][0]));
		char m = this->_validChannelMode(modes, validMode);
		if (!validMode)
			return (this->_get_message(this->_clients[fd]->getNick(), ERR_UNKNOWNMODE, std::string(1, m) + " :is unknown mode char to me.\r\n"));

		for (unsigned int i = 0; i < modes.size(); i++)
		{
			bool setMode = req.params[1][0] == '+' ? true : false;
			if (modes[i] == 'o')
			{
				if (req.params.size() < 3)
					return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameters given.\r\n"));
				if (!this->_channels[req.params[0]]->hasNickname(req.params[2]))
					return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOSUCHNICK, ":No such nick / channel.\r\n"));
				// :saeby!~laendrun@freenode-mkd.ame.j38t7t.IP MODE #chaton +o :laendrun
				if (setMode)
				{
					this->_channels[req.params[0]]->addOperator(this->_channels[req.params[0]]->getClient(req.params[2]));
					std::string ret = ":" + this->_clients[fd]->getNick();
					ret.append("!~" + this->_clients[fd]->getUser());
					ret.append("@" + this->_clients[fd]->getIp());
					ret.append(" MODE " + req.params[0] + " +o");
					ret.append(" :" + req.params[2]);
					ret.append("\r\n");
					this->sendMessageToChannelUsers(req.params[0], ret, fd);
				}
				else
				{
					this->_channels[req.params[0]]->removeOperator(this->_channels[req.params[0]]->getClient(req.params[2]));
					std::string ret = ":" + this->_clients[fd]->getNick();
					ret.append("!~" + this->_clients[fd]->getUser());
					ret.append("@" + this->_clients[fd]->getIp());
					ret.append(" MODE " + req.params[0] + " -o");
					ret.append(" :" + req.params[2]);
					ret.append("\r\n");
					this->sendMessageToChannelUsers(req.params[0], ret, fd);
				}
			}
			if (modes[i] == 'k' && req.params[1][0] == '+')
			{
				if (req.params.size() < 3)
					return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameters given.\r\n"));
				this->_channels[req.params[0]]->setPassword(req.params[2]);
				trailing = req.params[2];
			}
			else if (modes[i] == 'k' && req.params[1][0] == '-')
				this->_channels[req.params[0]]->setPassword("");

			if (modes[i] == 'l' && req.params[1][0] == '+')
			{
				if (req.params.size() < 3)
					return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameters given.\r\n"));
				this->_channels[req.params[0]]->setLimit(std::atoi(req.params[2].c_str()));
				trailing = req.params[2];
			}
			else if (modes[i] == 'l' && req.params[1][0] == '-')
				this->_channels[req.params[0]]->setLimit(0);
			chdModes.append(std::string(1, modes[i]));
			this->_channels[req.params[0]]->setMode(modes[i], setMode);
		}
		std::string ret = ":" + this->_clients[fd]->getNick();
		ret.append("!~" + this->_clients[fd]->getUser());
		ret.append("@" + this->_clients[fd]->getIp());
		ret.append(" MODE " + req.params[0]);
		if (trailing != "")
		{
			ret.append(" " + chdModes);
			ret.append(" :" + trailing);
		}
		else
			ret.append(" :" + chdModes);
		ret.append("\r\n");
		return (ret);
	}
	return ("");
}