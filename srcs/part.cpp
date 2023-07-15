/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Blaze <btchiman@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/15 19:10:03 by Blaze             #+#    #+#             */
/*   Updated: 2023/07/15 19:10:30 by Blaze            ###    42Lausanne.ch    */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string Server::_cmd_part(Request& req, int fd)
{
    std::string channel = req.params[0];
    std::string nick = this->_clients[fd]->getNick();
    std::string message = ":" + nick + " PART " + channel + "\r\n";

    if (this->_channels.find(channel) != this->_channels.end())
    {
        if (this->_channels[channel]->find(fd))
        {
            this->_channels[channel]->removeClient(this->_clients[fd]);
            if (this->_channels[channel]->empty())
                this->_channels.erase(channel);
            sendMessageToChannelUsers(channel, message, fd);
        }
        else
            return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOTONCHANNEL, ":You're not on that channel/r/n"));
    }
    else
        return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOSUCHCHANNEL, ":No such channel/r/n"));
    return ("PART " + channel + "\r\n");
}
