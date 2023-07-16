/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 07:34:50 by Blaze             #+#    #+#             */
/*   Updated: 2023/07/06 19:15:08 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"


std::string Server::_cmd_join(Request& req, int fd)
{
    std::string channel_name = req.params[0];
    std::string key = req.params[1];

    std::string nick = this->_clients[fd]->getNick();
    // ajouter user name et list of members
    std::string user_name = this->_clients[fd]->getUser();

    // check if the channel already exists
    if (this->_channels.find(channel_name) != this->_channels.end())
    {
        Channel* channel = this->_channels[channel_name];

        // Check if the channel requires a key and if the provided key is correct
        if (channel->hasPassword() && channel->getPassword() != key)
        {
            return (this->_get_message(nick, ERR_BADCHANNELKEY, "Cannot join channel " + channel_name + " (Incorrect key).\r\n"));
        }

        // Add customer to existing channel
        channel->addClient(this->_clients[fd]);
		this->_clients[fd]->join(channel_name);
        std::string members = this->_channels[channel_name]->getNicknamesList();
        std::string message = ":" + nick+  " JOIN " + channel_name  +"\r\n";
        // Send a notification message to other channel users
        sendMessageToChannelUsers(channel_name, message, fd);


        return (message);

    }
    else
    {
        // The channel does not exist, create a new channel

        Channel* channel = new Channel(channel_name);
        channel->addNickname(nick);

        // Check if the channel requires a key
        if (!key.empty())
        {
            channel->setPassword(key);
        }

        channel->addOperator(this->_clients[fd]);
        channel->setTopic("No topic is set");

        // Insert channel into channel list
        this->_channels.insert(std::make_pair(channel_name, channel));

        // Add customer to channel
        channel->addClient(this->_clients[fd]);
		this->_clients[fd]->join(channel_name);
        std::string members = this->_channels[channel_name]->getNicknamesList();
        std::string message = ":" + nick+  " JOIN " + channel_name + "\r\n";
        // Send a notification message to other channel users
        sendMessageToChannelUsers(channel_name, message, fd);


        return (message);
    }
}

