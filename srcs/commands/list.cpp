/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/20 16:35:05 by saeby             #+#    #+#             */
/*   Updated: 2023/07/20 18:16:54 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::vector<std::string> Server::_split_str(std::string& og_message, std::string delim)
{
	std::vector<std::string>	ret_vec;
	size_t						pos = 0;

	std::string token;
	while ((pos = og_message.find(delim)) != std::string::npos) {
		token = og_message.substr(0, pos);
		ret_vec.push_back(token);
		og_message.erase(0, pos + delim.length());
	}

	if (!ret_vec.size())
		ret_vec.push_back(og_message);
	return (ret_vec);
}

std::string	Server::_cmd_list(Request& req, int fd)
{
	// :ft_irc RPL_STARTLIST(321) <nick> Channel :Users Name
	// then for each channel => :ft_irc RPL_LIST(322) <nick> <channel_name> <user_count> :<topic>
	// :ft_irc RPL_ENNDLIST(323) <nick> :End of /LIST
	std::string	ret = ":" + this->_name + " " + RPL_LISTSTART + " " + this->_clients[fd]->getNick() + " Channel :Users Name\r\n";
	if (!req.params.size())
	{
		// no parameters, should list all channels with their topic
		// list secret channnel (+s) only if user joined it
		std::map<std::string, Channel *>	channels = this->_channels;
		for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
		{
			std::string	tmp = ":" + this->_name + " " + RPL_LIST + " " + this->_clients[fd]->getNick() + " ";
			if (it->second->getMode('s') && !it->second->hasNickname(this->_clients[fd]->getNick()))
				continue  ;
			tmp.append(it->second->getName() + " ");
			tmp.append(std::to_string(it->second->getUserCount()) + " ");
			if ((it->second->getMode('p') && it->second->hasNickname(this->_clients[fd]->getNick())) || !it->second->getMode('p'))
				tmp.append(it->second->getTopic());
			tmp.append("\r\n");
			ret.append(tmp);
		}
		ret.append(":" + this->_name + " ");
		ret.append(RPL_LISTEND);
		ret.append(" " + this->_clients[fd]->getNick() + " :End of /LIST\r\n");
		return (ret);
	}
	std::vector<std::string>	channels = this->_split_str(req.params[0], std::string(","));
	for (unsigned int i = 0; i < channels.size(); i++)
	{
		if (this->_channels.find(channels[i]) == this->_channels.end())
			continue ;
		std::string	tmp = ":" + this->_name + " " + RPL_LIST + " " + this->_clients[fd]->getNick() + " ";
		if (this->_channels[channels[i]]->getMode('s') && !this->_channels[channels[i]]->hasNickname(this->_clients[fd]->getNick()))
			continue ;
		tmp.append(this->_channels[channels[i]]->getName() + " ");
		tmp.append(std::to_string(this->_channels[channels[i]]->getUserCount()) + " ");
		if ((this->_channels[channels[i]]->getMode('p') && this->_channels[channels[i]]->hasNickname(this->_clients[fd]->getNick())) || !this->_channels[channels[i]]->getMode('p'))
			tmp.append(this->_channels[channels[i]]->getTopic());
		tmp.append("\r\n");
		ret.append(tmp);
	}
	ret.append(":" + this->_name + " ");
	ret.append(RPL_LISTEND);
	ret.append(" " + this->_clients[fd]->getNick() + " :End of channel /LIST\r\n");
	return (ret);
}