/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 04:09:21 by Blaze             #+#    #+#             */
/*   Updated: 2023/07/07 17:00:33 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"


std::string Server::_cmd_privmsg(Request& req, int fd)
{
	std::string trailing = req.trailing;
	std::string target = req.params[0];
	std::string nick = this->_clients[fd]->getNick();

	std::string message = ":" + nick + " PRIVMSG " + target + " " + trailing + "\n";

	if (this->_channels.find(target) != this->_channels.end())
			sendMessageToChannelUsers(target, message, fd);
	else if (std::find (this->_nicknames.begin(), this->_nicknames.end(), req.params[0]) != this->_nicknames.end())
		sendPrivateMessage(target, message, fd);
	else
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOSUCHNICK, ":No such nick / channel/r/n"));
	return ("");
}

void Server::sendPrivateMessage(const std::string& userNickname, const std::string& message, int fd)
{
    int send_ret;
    Client* targetClient = NULL;

    // looking for the client by nickname
    std::map<int, Client*>::iterator it;
    for (it = this->_clients.begin(); it != this->_clients.end(); ++it)
    {
        Client* client = it->second;
        if (client->getNick() == userNickname)
        {
            targetClient = client;
            break;
        }
    }

    if (targetClient != NULL)
    {
        int target_fd = targetClient->getFd();
        if (target_fd != fd)
        {
            send_ret = send(target_fd, message.c_str(), message.length(), 0);
            if (send_ret < 0)
                std::cerr << "send() error: " << strerror(errno) << std::endl;
        }
    }
    else
    {
        std::cerr << "User not found: " << userNickname << std::endl;
    }
}

void Server::sendMessageToChannelUsers(const std::string& channel_name, const std::string& message, int fd)
{
    int send_ret;
    Channel *channel = this->_channels[channel_name];
    std::vector<Client*> clients = channel->getClients();
    std::vector<Client*>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        Client *client = *it;
        int client_fd = client->getFd();
        std::string client_nick = client->getNick();
        if (client_fd != fd){
            send_ret = send(client_fd, message.c_str(), message.length(), 0);
            if (send_ret < 0)
                std::cerr << "send() error: " << strerror(errno) << std::endl;
        }
    }
}
