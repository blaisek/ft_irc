/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   whois.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 23:23:47 by Blaze             #+#    #+#             */
/*   Updated: 2023/07/20 11:21:16 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

// irc whois cmd
std::string Server::_cmd_whois(Request &req, int fd) {

    std::string nick;

    if (req.params[0].empty())
        nick = this->_clients[fd]->getNick();
    else
        nick = req.params[0];

    if (this->_clients.find(this->_fdByNick(nick)) == this->_clients.end())
        return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOSUCHNICK, ":No such nick\r\n"));

    if(this->_clients[this->_fdByNick(nick)]->getMode('i') && !this->_clients[fd]->getMode('o'))
        return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOPRIVILEGES, ":Permission Denied- You're not an IRC operator\r\n"));

    std::string msg = ":" + this->_name + " " + RPL_WHOISUSER + " " + this->_clients[fd]->getNick() + " " +
            this->_clients[this->_fdByNick(nick)]->getNick() + " " + this->_clients[this->_fdByNick(nick)]->getUser() +
            " " + this->_clients[this->_fdByNick(nick)]->getHost() + " * :" + this->_clients[this->_fdByNick(nick)]->getFullName() + "\r\n";
    msg += ":" + this->_name + " " + RPL_WHOISSERVER + " " + this->_clients[fd]->getNick() + " " + this->_clients[this->_fdByNick(nick)]->getNick() +
            " " + this->_name + " :" + this->_name + "\r\n";
    msg += ":" + this->_name + " " + RPL_WHOISCHANNELS + " " + this->_clients[fd]->getNick() + " " + this->_clients[this->_fdByNick(nick)]->getNick() +
            " :" + this->_clients[this->_fdByNick(nick)]->getChannelsNames() + "\r\n";
    msg += ":" + this->_name + " " + RPL_WHOISOPERATOR + " " + this->_clients[fd]->getNick() + " " + this->_clients[this->_fdByNick(nick)]->getNick() +
            " :is an IRC operator\r\n";
    msg += ":" + this->_name + " " + RPL_WHOISIDLE + " " + this->_clients[fd]->getNick() + " " + this->_clients[this->_fdByNick(nick)]->getNick() +
            " " + std::to_string(this->_clients[this->_fdByNick(nick)]->getIdleTime()) + " " +
            this->_clients[this->_fdByNick(nick)]->getSignOnTime() + " :seconds idle, signon time\r\n";
    msg += ":" + this->_name + " " + RPL_ENDOFWHOIS + " " + this->_clients[fd]->getNick() + " " +
            this->_clients[this->_fdByNick(nick)]->getNick() + " :End of WHOIS list\r\n";
    return (msg);
}
