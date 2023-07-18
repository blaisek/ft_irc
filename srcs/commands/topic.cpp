/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Blaze <btchiman@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 11:12:54 by Blaze             #+#    #+#             */
/*   Updated: 2023/07/18 11:43:44 by Blaze            ###    42Lausanne.ch    */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string Server::_cmd_topic(Request& req, int fd)
{

    std::string topic = req.trailing;
    std::string channel = req.params[0];
    std::string nick = this->_clients[fd]->getNick();
    std::string msg;

    if(this->_channels.find(channel) == this->_channels.end())
        return (this->_get_message(nick, ERR_NOSUCHCHANNEL, "No such channel"));

    if(!topic.empty() && this->_channels[channel]->getMode('t') && !this->_channels[channel]->isOp(nick))
        return (this->_get_message(nick, ERR_CHANOPRIVSNEEDED, "You're not channel operator"));

    else if(!topic.empty() && this->_channels[channel]->getMode('t') && this->_channels[channel]->isOp(nick))
    {
        this->_channels[channel]->setTopic(topic);
        msg = ":" + nick + " TOPIC " + channel + " :" + topic + "\r\n";
        this->sendMessageToChannelUsers(channel,msg,fd);
        return msg;
    }

    else if(!topic.empty() && !this->_channels[channel]->getMode('t'))
    {
        this->_channels[channel]->setTopic(topic);
        msg = ":" + nick + " TOPIC " + channel + " :" + topic + "\r\n";
        this->sendMessageToChannelUsers(channel,msg,fd);
        return msg;
    }

    else
    {
        msg = ":" + nick + " TOPIC " + channel + " :" + this->_channels[channel]->getTopic() + "\r\n";
        this->sendMessageToChannelUsers(channel,msg,fd);
        return msg;
    }
}
