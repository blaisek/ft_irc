
#include "irc.hpp"

std::string	Server::_cmd_invite(Request& req, int fd)
{
	if (req.params.size() < 2)
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NEEDMOREPARAMS, ":Not enough parameteres givent.\r\n"));

	// check that user exist
	if (std::find(this->_nicknames.begin(), this->_nicknames.end(), req.params[0]) == this->_nicknames.end())
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOSUCHNICK, ":" + req.params[0]));

	std::map<std::string, Channel *>::const_iterator it = this->_channels.begin();
	while (it != this->_channels.end())
	{
		if (it->first == req.params[1])
			break;
		it++;
	}

	if (it != this->_channels.end())
	{
		if (it->second->getMode('i'))
		{
			if (!it->second->isOp(this->_clients[fd]->getNick()))
				return (this->_get_message(this->_clients[fd]->getNick(), ERR_CHANOPRIVSNEEDED, ":You need chanop privileges to invite peopel.\r\n"));
			it->second->addInvited(req.params[0]);
			std::string	mes = ":" + this->_clients[fd]->getNick() + " INVITE " + req.params[0] + " " + req.params[1] + "\r\n";
			if (send(this->_fdByNick(req.params[0]), mes.c_str(), mes.length(), 0) < 0)
				std::cerr << "send() error: " << strerror(errno) << std::endl;
			return (":" + this->_name + " " + RPL_INVITING + " " + this->_clients[fd]->getNick() + " " + req.params[0] + " :" + req.params[1] + "\r\n");

		}
	}
	std::string	mes = ":" + this->_clients[fd]->getNick() + " INVITE " + req.params[0] + " " + req.params[1] + "\r\n";
	if (send(this->_fdByNick(req.params[0]), mes.c_str(), mes.length(), 0) < 0)
		std::cerr << "send() error: " << strerror(errno) << std::endl;
	return (this->_get_message(this->_clients[fd]->getNick(), RPL_INVITING, req.params[0] + " " + req.params[1]));
}
