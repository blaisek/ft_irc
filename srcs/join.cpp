/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Blaze <btchiman@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/02 07:34:50 by Blaze             #+#    #+#             */
/*   Updated: 2023/07/02 07:35:05 by Blaze            ###    42Lausanne.ch    */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string Server::_cmd_join(Request& req, int fd)
{
    std::string channel_name = req.params[0];

    std::string nick = this->_clients[fd]->getNick();
    if (this->_channels.find(channel_name) == this->_channels.end())
    {
        Channel *channel = new Channel(channel_name);
        channel->addNickname(nick);
        channel->addOperator(this->_clients[fd]);
        channel->setTopic("No topic is set");
        this->_channels.insert(std::make_pair(channel_name, channel));
    }
    this->_channels[channel_name]->addClient(this->_clients[fd]);
    std::string message = ":" + nick + " JOIN " + channel_name + "\n";

    // Envoyer le message à tous les utilisateurs du même canal
    Channel *channel = this->_channels[channel_name];
    std::vector<Client*> clients = channel->getClients();
    std::vector<Client*>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        Client *client = *it;
        int client_fd = client->getFd();
        if (client_fd != fd)
            send(client_fd, message.c_str(), message.length(), 0);
    }

    return ("");
}

