
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


