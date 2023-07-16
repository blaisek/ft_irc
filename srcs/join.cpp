/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 07:34:50 by Blaze             #+#    #+#             */
/*   Updated: 2023/07/16 17:14:13 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"


std::string Server::_cmd_join(Request& req, int fd)
{
    std::string channel_name = req.params[0];
    std::string key = req.params[1];

    std::string nick = this->_clients[fd]->getNick();

    // check if the channel already exists
    if (this->_channels.find(channel_name) != this->_channels.end())
    {
        Channel* channel = this->_channels[channel_name];

        // Check if the channel requires a key and if the provided key is correct
        if (channel->hasPassword() && channel->getPassword() != key)
        {
            return (this->_get_message(nick, ERR_BADCHANNELKEY, ":Cannot join channel " + channel_name + " (Incorrect key).\r\n"));
        }

		if (channel->getMode('i'))
		{
			// invite only channel
			if (!channel->isInvited(this->_clients[fd]->getNick()))
				return (this->_get_message(nick, ERR_INVITEONLYCHAN, ":" + channel_name + "\r\n"));
		}
        // Add customer to existing channel
        channel->addClient(this->_clients[fd]);
		this->_clients[fd]->join(channel_name);

        // Send a notification message to other channel users
        std::string message = ":" + nick + " JOIN " + channel_name + "\n";
        sendMessageToChannelUsers(channel_name, message, fd);

        return ("");
    }
    else
    {
        // The channel does not exist, create a new channel

        Channel* channel = new Channel(channel_name);
        channel->addNickname(nick);

        channel->addOperator(this->_clients[fd]);
        channel->setTopic("No topic is set");

        // Insert channel into channel list
        this->_channels.insert(std::make_pair(channel_name, channel));

        // Add customer to channel
        channel->addClient(this->_clients[fd]);
		this->_clients[fd]->join(channel_name);

        // Send a notification message to other channel users
        std::string message = ":" + nick + " JOIN " + channel_name + "\n";
        sendMessageToChannelUsers(channel_name, message, fd);

        return ("");
    }
}

