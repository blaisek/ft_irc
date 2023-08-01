
#include "irc.hpp"

std::string	Server::_cmd_notice(Request& req, int fd)
{
	std::string	mes = ":" + this->_clients[fd]->getNick() + " NOTICE " + req.params[0] + " " + req.trailing + "\r\n";
	
	if (this->_channels.find(req.params[0]) != this->_channels.end())
		this->_sendNoticeToChannel(req.params[0], mes, fd);
	else if (std::find(this->_nicknames.begin(), this->_nicknames.end(), req.params[0]) != this->_nicknames.end())
		this->_sendNoticeToUser(req.params[0], mes, fd);
	else
		return (this->_get_message(this->_clients[fd]->getNick(), ERR_NOSUCHNICK, ":No such nick / channel\r\n"));
	return ("");
}

void	Server::_sendNoticeToChannel(const std::string& target, const std::string& mes, int fd)
{
	(void) fd;
	std::vector<Client *>	clients = this->_channels[target]->getClients();
	
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		Client*	client = *it;
		if (send(client->getFd(), mes.c_str(), mes.length(), 0) < 0)
			std::cerr << "send() error: " << strerror(errno) << std::endl;
	}
}

void	Server::_sendNoticeToUser(const std::string& target, const std::string& mes, int fd)
{
	(void) fd;
	int target_fd = this->_fdByNick(target);
	if (send(target_fd, mes.c_str(), mes.length(), 0) < 0)
		std::cerr << "send() error: " << strerror(errno) << std::endl;
}
