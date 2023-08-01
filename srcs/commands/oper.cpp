
#include "irc.hpp"

std::string	Server::_cmd_oper(Request& req, int fd)
{
	(void) req;
	(void) fd;
	if (req.params.size() < 2)
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameters given.\r\n"));
	if (std::find(this->_nicknames.begin(), this->_nicknames.end(), req.params[0]) == this->_nicknames.end())
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOSUCHNICK, ":No such nick\r\n"));
	else
	{
		if (req.params[1] != this->_oper_pass)
			return (this->_get_message(this->_clients[fd]->getNick(), ERR_PASSWDMISMATCH, ":Wrong operator password.\r\n"));
		this->_clients[fd]->setMode('o', true);
		// send a mode change reply for the client too here
		std::string	ret = this->_get_message(this->_clients[fd]->getNick(), RPL_YOUREOPER, ":You're now an IRC operator.\r\n");
		ret.append(":" + this->_clients[fd]->getNick() + " MODE " + this->_clients[fd]->getNick() + " +o\r\n");
		return (ret);
	}
	return ("");
}
