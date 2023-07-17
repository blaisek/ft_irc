/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 05:56:31 by Blaze             #+#    #+#             */
/*   Updated: 2023/07/17 20:11:15 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "irc.hpp"

void Server::sendJoinResponses(int fd, const std::string& nick, const std::string& user_name, const std::string& ip,
                       const std::string& channel_name, const std::string& members,
                       const std::string& hostname)
{
    std::string message = ":" + nick+ "!~"+ user_name + '@'+ ip + " JOIN " + channel_name + "\r\n";
    std::string response;
    std::string topic = this->_channels[channel_name]->getTopic();
    // 1
    response = message;
    send(fd, response.c_str(), response.length(), 0);

    // 2
    response = ":" + hostname + " 353 " + nick + " @ " + channel_name + " :" + members + "\r\n";
    send(fd, response.c_str(), response.length(), 0);

    // 3
    response = ":" + hostname + " 366 " + nick + " " + channel_name + " :End of NAMES list\r\n";
    send(fd, response.c_str(), response.length(), 0);

    // 4
    response = ":" + hostname + " 324 " + nick + " " + channel_name + "\r\n";
    send(fd, response.c_str(), response.length(), 0);

    // 5
    if (!topic.empty())
    {
        response = ":" + hostname + " 332 " + user_name + " " + channel_name + " :" + topic + "\r\n";
        send(fd, response.c_str(), response.length(), 0);
    }

    // 6
    response = ":" + hostname + " 333 " + user_name + " " + channel_name + " " + nick + " 0\r\n";
    send(fd, response.c_str(), response.length(), 0);

    // Send a notification message to other channel users
    sendMessageToChannelUsers(channel_name, message, fd);
}


std::string Server::_cmd_join(Request& req, int fd)
{
    std::string channel_name = req.params[0];
    std::string key = req.params[1];

    std::string nick = this->_clients[fd]->getNick();
    std::string hostname = this->_clients[fd]->getHost();
    std::string user_name = this->_clients[fd]->getUser();
    std::string ip = this->_clients[fd]->getIp();


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
        std::string members = this->_channels[channel_name]->getNicknamesList();
        sendJoinResponses(fd, nick, user_name, ip, channel_name, members, hostname);
        return"";
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
        std::string members = this->_channels[channel_name]->getNicknamesList();
        sendJoinResponses(fd, nick, user_name, ip, channel_name, members, hostname);
        return"";
    }
}

