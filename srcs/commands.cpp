/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/24 19:44:58 by saeby             #+#    #+#             */
/*   Updated: 2023/07/07 16:06:56 by saeby            ###   ########.fr       */
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
        return (this->_get_message(this->_clients[fd]->getNick(), ERR_PASSWDMISMATCH, "You need to authenticate first.\r\n"));

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
        }
    }

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
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_PASSWDMISMATCH, "Password mismatch.\r\n"));
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
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_PASSWDMISMATCH, ":You need to authenticate first.\r\n"));
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

// 1. check if user is registered
// 2. no param => ERR_NEEDMOREPARAMS
// 3. one param => based on what irc.freenode.org does
//     3.1 => if 1st param is current nick, return current user's mode
//     3.2 => if 1st param is a nick but not current user's nick and current user is not srv operator => cannot view mode of other users
//     3.3 => if 1st param is a nick but not current user's nick and current user is srv operator => display nick's modes
//     3.4 => if 1st param is a channel name => display channel's modes
//     3.5 => else => error not a nick/channel
// 4. two params
//     4.1 => if first param is a nickname
//          4.1.1 => check if nick is current nick
//               4.1.1.1 => if second param starts with +
//               4.1.1.2 => if second param starts with -
//     4.2 => if first param is a channel name
std::string	Server::_cmd_mode(Request& req, int fd)
{
	// 1
	if (!this->_clients[fd]->getReg())
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOTREGISTERED, ":You must be registered to do this.\r\n"));
	// 2
	if (req.params.size() == 0)
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameters given.\r\n"));
	// 3
	if (req.params.size() == 1)
	{
		// 3.1
		if (req.params[0] == this->_clients[fd]->getNick())
			return (this->_get_message(this->_clients[fd]->getNick(), RPL_UMODEIS, this->_clients[fd]->getModes()));
		// 3.2 & 3.3
		if (std::find(this->_nicknames.begin(), this->_nicknames.end(), req.params[0]) != this->_nicknames.end())
		{
			if (this->_clients[fd]->isOp())
				return (this->_clients[this->_fdByNick(req.params[0])]->getModes());
			else
				return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOPRIVILEGES, ":You do not have rights to see other users' mode.\r\n"));
		}
		// 3.4 
		if (this->_channels.find(req.params[0]) != this->_channels.end())
			return (this->_channels[req.params[0]]->getModes());
	}
	else
	{
		bool		validMode = true;
		std::string	chdModes;
		if (std::find(this->_nicknames.begin(), this->_nicknames.end(), req.params[0]) != this->_nicknames.end())
		{
			// nick exists
			// check if first char is + or - else return => ERR_UNKNOWNMODE
			if (req.params[1][0] != '-' && req.params[1][0] != '+')
				return (this->_get_message(this->_clients[fd]->getNick(), ERR_UNKNOWNMODE, std::string(1, req.params[1][0]) + " :is unknown mode char to me.\r\n"));
			std::vector<char>	modes = this->_splitModes(req.params[1]);
			chdModes.append(std::string(1, req.params[1][0]));
			char m = this->_validUserMode(modes, validMode);
			if (!validMode)
				return (this->_get_message(this->_clients[fd]->getNick(), ERR_UNKNOWNMODE, std::string(1, m) + " :is unknown mode char to me.\r\n"));
			for (unsigned int i = 0; i < modes.size(); i++)
			{
				bool setMode = req.params[1][0] == '+' ? true : false;
				if (setMode && modes[i] == 'o')
					continue ;
				chdModes.append(std::string(1, modes[i]));
				this->_clients[fd]->setMode(modes[i], setMode);
			}
			return (":" + this->_clients[fd]->getNick() + " MODE " + this->_clients[fd]->getNick() + " " + chdModes + "\r\n");
		}
		else if (this->_channels.find(req.params[0]) != this->_channels.end())
		{
			return ("Channel exists\r\n");
			// channel exists
		}
	}
	return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOSUCHNICK, ":No such nick / channel\r\n"));
	return ("");
}

std::string	Server::_cmd_quit(Request& req, int fd)
{
	// segfault => probably caused in the sendMessageToChannelUsers because
	// channel tries to contact a user that has already left...
	// make client leave all channels before sending the message
	unsigned int i = 0;
	while (this->_poll_fds[i].fd != fd)
		i++;

	// form quit message
	std::string	quitmes;

	std::string nick = this->_clients[fd]->getNick();

	quitmes.append(":" + nick);
	quitmes.append(" QUIT " + req.trailing + "\r\n");

	std::vector<std::string> chans = this->_clients[fd]->getChans();
	for (unsigned int i = 0; i < chans.size(); i++)
	{
		this->_channels[chans[i]]->removeClient(this->_clients[fd]);
		this->sendMessageToChannelUsers(chans[i], quitmes, fd);
		this->_clients[fd]->leave(chans[i]);
	}

	this->_nicknames.erase(std::remove(this->_nicknames.begin(), this->_nicknames.end(), nick), this->_nicknames.end());
	this->_remove_client(i);
	return ("");
}
