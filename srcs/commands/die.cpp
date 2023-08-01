
#include "irc.hpp"

std::string	Server::_cmd_die(Request& req, int fd)
{
	(void) req;
	if (!this->_clients[fd]->isAuth())
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_PASSWDMISMATCH, ":You need to authenticate first.\r\n"));

	// :ft_irc ERR_NOPRIVILEGES <nick> :You do not have the required operator privileges
	if (!this->_clients[fd]->getMode('o'))
	{
		std::string ret = ":" + this->_name + " ";
		ret.append(ERR_NOPRIVILEGES);
		ret.append(" " + this->_clients[fd]->getNick() + " ");
		ret.append(":You do not have the required operator privileges.\r\n");
		return (ret);
	}
	else
		exit(0);
	return ("\r\n");
}
