/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 19:44:58 by saeby             #+#    #+#             */
/*   Updated: 2023/07/20 13:05:02 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"


// 1. Check if user authenticated with password -> ERR_PASSWDMISMATCH (if no password or wrong password, in this case no password)
// 2. Check if enough parameters -> ERR_NONICKNAMEGIVEN (no nickname provided)
// 3. Check for erroneus nickname -> ERR_ERRONEUSNICKNAME
//     if (!alnum && == '-' && == '\r') => erroneus
// 4. Check if nick used -> ERR_NICKNAMEINUSE (already used)
// 5. Save nick in client
// 6. Save nickname in server's used nicknames
// 7. Check if USER already saved (in case of nickname change) => set user identity <nick>!<user>@<host>
//     if user identity is saved, reply with the RPL_WELCOME code
//     else simply return an empty message, the RPL_WELCOME will be sent when going through the USER command

std::string Server::_cmd_nick(Request& req, int fd)
{
    (void) req;
    // 1
    if (!this->_clients[fd]->isAuth())
		return (":" + this->_name + " " + ERR_PASSWDMISMATCH + " * :You need to authenticate first.\r\n");

    // 2
    if (req.params.size() < 1)
        return (this->_get_message(this->_clients[fd]->getNick(), ERR_NONICKNAMEGIVEN, "No nickname given.\r\n"));

    // 3
    size_t i = 0;
    std::string nick = req.params[0];
    while (nick[i])
    {
        int c = nick[i];
        if (!isalnum(c) && c != '-' && c != '[' && c != ']' && c != '{' && c != '}' && c != '\\' && c != '`' && c != '^' && c != '_')
            return (this->_get_message(this->_clients[fd]->getNick(), ERR_ERRONEUSNICKNAME, nick + " :Erroneous nickname\r\n"));
        i++;
    }

    // 4
    std::vector<std::string>::iterator it = std::find(this->_nicknames.begin(), this->_nicknames.end(), req.params[0]);
    if (it != this->_nicknames.end())
    {
        // If the nickname is already in use by another client
        if (this->_clients[fd]->getNick() != req.params[0])
        {
            return (this->_get_message(this->_clients[fd]->getNick(), ERR_NICKNAMEINUSE, nick + " :Nickname already used.\r\n"));
        }
        else
        {
            // If the nickname is the same as the current one
            return (this->_get_message(this->_clients[fd]->getNick(), RPL_NICKTRACE, nick + " :Nickname unchanged.\r\n"));
        }
    }

    // 5
    std::string oldNick = this->_clients[fd]->getNick();
    this->_clients[fd]->setNick(req.params[0]);
    this->_nicknames.push_back(this->_clients[fd]->getNick());

    // Update the nickname in all channels if the client is a member
    for (std::map<std::string, Channel *>::iterator channelIt = this->_channels.begin(); channelIt != this->_channels.end(); ++channelIt)
    {
        if (channelIt->second->hasNickname(oldNick))
        {
            channelIt->second->removeNickname(oldNick);
            channelIt->second->addNickname(nick);
			if (channelIt->second->isOp(oldNick))
			{
				channelIt->second->removeOperator(channelIt->second->getClient(oldNick));
				channelIt->second->addOperator(channelIt->second->getClient(nick));
			}
        }
    }

	// remove nickname from server's nick list
	this->_nicknames.erase(std::remove(this->_nicknames.begin(), this->_nicknames.end(), oldNick), this->_nicknames.end());

    // 6
    if (!this->_clients[fd]->getUser().empty())
    {
        this->_clients[fd]->setIdentity(this->_clients[fd]->getNick() + "!" + this->_clients[fd]->getUser() + "@" + this->_clients[fd]->getHost());
        this->_clients[fd]->setReg(true);
        return (this->_get_message(this->_clients[fd]->getNick(), RPL_WELCOME, "You're now registered! " + this->_clients[fd]->getIdentity() + "\r\n"));
    }

    return ("");
}




// 1. check param count => ERR_NEEDMOREPARAMS
// 2. check if client already registered => ERR_ALREADYREGISTERED
// 3. compare given password to server password => ERR_PASSWDMISMATCH
// 4. set client to authenticated
// 5. return ("")
std::string	Server::_cmd_pass(Request& req, int fd)
{
	// 1
	if (req.params.size() < 1)
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, "Not enough parameters given.\r\n"));
	// 2
	if (this->_clients[fd]->getReg())
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_ALREADYREGISTRED, "Already registered.\r\n"));
	// 3
	if (this->_pass != req.params[0])
		return (":" + this->_name + " " + ERR_PASSWDMISMATCH + " * :Password mismatch.\r\n");
	this->_clients[fd]->setAuth(true);
	return ("");
}

// 1. check if client authenticated
// 2. check if user already registered
// 3. check params count (< 3)
// 4. set username
// 5. set fullname
// 6. check if NICK already saved => set user identity
//     if user identity is saved, reply with RPL_WELCOME
// USER <username> <hostname> <servername> <realname>
std::string	Server::_cmd_user(Request& req, int fd)
{
	// 1
	if (!this->_clients[fd]->isAuth())
		return (":" + this->_name + " " + ERR_PASSWDMISMATCH + " * :You need to authenticate first.\r\n");
	// 2
	if (this->_clients[fd]->getReg())
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_ALREADYREGISTRED, ":Already registered.\r\n"));
	// 3
	if (req.params.size() < 3)
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameters given.\r\n"));
	// 4
	this->_clients[fd]->setUser(req.params[0]);
	this->_clients[fd]->setHost(req.params[2]);
	this->_clients[fd]->setFullName(req.trailing);
	if (this->_clients[fd]->getNick() != "")
	{
		this->_clients[fd]->setIdentity(this->_clients[fd]->getNick() + "!" + this->_clients[fd]->getUser() + "@" + this->_clients[fd]->getHost());
		this->_clients[fd]->setReg(true);
		return (this->_get_message(this->_clients[fd]->getNick(), RPL_WELCOME, ":You're now registered! " + this->_clients[fd]->getIdentity() + "\r\n"));
	}
	return ("");
}

// 1. check params count (< 1)
// 2. reply with PONG message
std::string	Server::_cmd_ping(Request& req, int fd)
{
	if (req.params.size() < 1)
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameteres givent.\r\n"));
	return ("PONG " + req.params[0] + "\r\n");
}

std::string	Server::_cmd_quit(Request& req, int fd)
{
	std::string	quitmes;
	std::string nick = this->_clients[fd]->getNick();

	quitmes.append(":" + nick);
	quitmes.append(" QUIT " + req.trailing + "\r\n");

	std::vector<std::string> chans = this->_clients[fd]->getChans();
	for (unsigned int j = 0; j < chans.size(); j++)
	{
		this->_channels[chans[j]]->removeClient(this->_clients[fd]);
		this->sendMessageToChannelUsers(chans[j], quitmes, fd);
		this->_clients[fd]->leave(chans[j]);
	}

	this->_nicknames.erase(std::remove(this->_nicknames.begin(), this->_nicknames.end(), nick), this->_nicknames.end());
	close(fd);
	this->_remove_client(fd);
	return ("");
}
