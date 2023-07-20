/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 16:02:39 by Blaze             #+#    #+#             */
/*   Updated: 2023/07/20 15:50:43 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string Server::_cmd_kick(Request& req, int fd)
{
	std::string channel_name = req.params[0];
	std::string target = req.params[1];
    int target_fd = this->_fdByNick(target);
	std::string fnick = this->_clients[fd]->getNick() + "!~" + this->_clients[fd]->getUser() + "@" + this->_clients[fd]->getIp();
    std::string nick = this->_clients[fd]->getNick();
	std::string msg;

    if(this->_channels.find(channel_name) == this->_channels.end())
    {
        msg = "No such channel";
        send(fd, msg.c_str(), msg.length(), 0);
        return (this->_get_message(nick, ERR_NOSUCHCHANNEL,msg ));
    }
    if(!this->_channels[channel_name]->hasNickname(nick))
    {
        msg = "You're not on that channel";
        send(fd, msg.c_str(), msg.length(), 0);
        return (this->_get_message(nick, ERR_NOTONCHANNEL, msg));
    }
    if(!this->_channels[channel_name]->hasNickname(target))
    {
        msg = "User not in channel";
        send(fd, msg.c_str(), msg.length(), 0);
        return (this->_get_message(nick, ERR_USERNOTINCHANNEL, msg));
    }
    if(!this->_channels[channel_name]->isOp(nick))
    {
        msg = "You're not channel operator";
        send(fd, msg.c_str(), msg.length(), 0);
        return (this->_get_message(nick, ERR_CHANOPRIVSNEEDED, "msg"));
    }
    this->_channels[channel_name]->removeNickname(target);
	if (!req.trailing.empty())
		msg = ":" + fnick + " KICK " + channel_name + " " + target + " :" + req.trailing + "\r\n";
	else
	    msg = ":" + fnick + " KICK " + channel_name + " " + target + "\r\n";
    // send(target_fd, msg.c_str(), msg.length(), 0);
    this->sendMessageToChannelUsers(channel_name,msg,fd);
    this->_clients[target_fd]->leave(channel_name);
    this->_channels[channel_name]->removeUser(this->_clients[target_fd]->getNick());
    return (msg);
}
