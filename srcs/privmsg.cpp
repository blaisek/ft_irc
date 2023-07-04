/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Blaze <btchiman@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 04:09:21 by Blaze             #+#    #+#             */
/*   Updated: 2023/07/04 04:15:20 by Blaze            ###    42Lausanne.ch    */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string Server::_cmd_privmsg(Request& req, int fd)
{
    std::string trailing = req.trailing;
    std::string channel_name = req.params[0];
    std::string nick = this->_clients[fd]->getNick();

    std::string message = ":" + nick + " " + trailing + "\n";

    if (this->_channels.find(channel_name) != this->_channels.end())
        sendMessageToChannelUsers(channel_name, message, fd);
    else
        return "error: channel not found" + channel_name + "\n";

    return "";
}

