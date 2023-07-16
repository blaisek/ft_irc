/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 15:05:34 by saeby             #+#    #+#             */
/*   Updated: 2023/07/06 18:07:47 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// Default constructor
Server::Server(void) : _name(), _pass(), _socket(0), _online(0), _max_online(10), _poll_fds(nullptr), _clients() {}

Server::Server(std::string name, std::string pass, std::string port, int max) : _name(name), _pass(pass), _max_online(max)
{
	this->_poll_fds = new struct pollfd[this->_max_online];
	this->_init_socket(port);
	this->_poll_fds[0].fd = this->_socket;
	this->_poll_fds[0].events = POLLIN;
	this->_online = 1;
}

// Copy constructor
Server::Server(const Server &other)
{
	(void) other;
	return ;
}

// Assignment operator overload
Server &Server::operator=(const Server &other)
{
	(void) other;
	return (*this);
}

// Destructor
Server::~Server(void)
{
	return ;
}

void	Server::start(void)
{
	while (1)
	{
		int count = poll(this->_poll_fds, this->_online, -1);
		if (count < 0)
			error("poll() error.", 1);
		for (int i = 0; i < this->_online; i++)
		{
			if (this->_poll_fds[i].revents & POLLIN)
			{
				if (this->_poll_fds[i].fd == this->_socket)
					this->_create_client();
				else
					this->_handle_request(i);
			}
		}
	}
}

void	Server::_init_socket(std::string port)
{
	int enable = 1;
	struct sockaddr_in	addr;

	bzero((char *) &addr, sizeof(addr));
	
	this->_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket < 0)
		error("socket() error", 1);
	setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(std::atoi(port.c_str()));

	if (bind(this->_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		error("bind() error", 1);

	if (listen(this->_socket, this->_max_online) < 0)
		error("listen() error", 1);
}

void	Server::_create_client(void)
{
	struct sockaddr	client_addr;
	socklen_t		addr_len;

	addr_len = sizeof(client_addr);

	int fd = accept(this->_socket, &client_addr, &addr_len);
	if (fd < 0)
		std::cerr << "accept() error: " << strerror(errno) << std::endl;
	// max online is "only" a way to allocate enough memory, so we'll double that number if needed
	if (this->_online == this->_max_online)
	{
		this->_max_online *= 2;
		this->_poll_fds = (struct pollfd *)realloc(this->_poll_fds, this->_max_online); //possible mistake here
	}
	// now we need to add the new file descriptor to the list of all poll fds
	this->_poll_fds[this->_online].fd = fd;
	this->_poll_fds[this->_online].events = POLLIN;
	this->_online++;
	
	// and of course, insert a new Client into the servers clients map
	this->_clients.insert(std::pair<int, Client *>(fd, new Client(fd)));

	std::cout << "[" << timestamp() << "]: new connection from " << inet_ntoa(((struct sockaddr_in *)&client_addr)->sin_addr) << " on file descriptor " << fd << std::endl;
	std::string message = "Welcome to our little irc server\n";
	if (send(fd, message.c_str(), message.length(), 0) < 0)
		std::cerr << "send() error: " << strerror(errno) << std::endl;
	
	std::cout << *this << std::endl;
}

void	Server::_remove_client(int i)
{
	close(this->_poll_fds[i].fd);
	this->_poll_fds[i] = this->_poll_fds[this->_online - 1];
	this->_clients.erase(this->_poll_fds[i].fd);
	this->_online -= 1;
}

void	Server::_handle_request(int i)
{
	IrcParser				parser;
	std::vector<Request>	reqs;
	int						fd = this->_poll_fds[i].fd;
	int						n = 0;
	char					buf[512];

	n = recv(fd, &buf, sizeof(buf), 0);

	if (n == 0)
	{
		std::cout << "[" << timestamp() << "]: file descriptor: " << fd << " disconnected." << std::endl;
		this->_remove_client(i);
	}
	else if (n < 0)
	{
		std::cerr << "ERROR: recv() error: " << strerror(errno) << std::endl;
		this->_remove_client(i);
	}
	else
	{
		// parse request
		reqs = parser.parse(std::string(buf));
		for (unsigned int j = 0; j < reqs.size(); j++)
		{
			std::cout << reqs[j] << std::endl;
			if (reqs[j].cmd == "CAP")
				continue ;
			std::string ret = this->_reply(reqs[j], fd);
			std::cout << "Reply: " << ret << std::endl;
			if (reqs[j].cmd != "QUIT")
			{
				if (send(fd, ret.c_str(), ret.length(), 0) < 0)
					std::cerr << "send() error: " << strerror(errno) << std::endl;
			}
		}
	}
	// reset buffer so its empty the next time around
	bzero(&buf, sizeof(buf));

}

std::string	Server::_get_message(std::string nick, std::string code, std::string message)
{
	if (nick.empty())
		nick = "*";
	return (":" + this->_name + " " + code + " " + nick + " " + message + "\n");
}

std::string	Server::_reply(Request req, int fd)
{
	if (!req.valid)
		return ("Invalid request\r\n");
	else if (req.cmd == "NICK")
		return (this->_cmd_nick(req, fd));
	else if (req.cmd == "PASS")
		return (this->_cmd_pass(req, fd));
	else if (req.cmd == "USER")
		return (this->_cmd_user(req, fd));
	else if (req.cmd == "PING")
		return (this->_cmd_ping(req, fd));
	else if (req.cmd == "JOIN")
		return (this->_cmd_join(req, fd));
	else if (req.cmd == "PRIVMSG")
		return (this->_cmd_privmsg(req, fd));
	else if (req.cmd == "MODE")
		return (this->_cmd_mode(req, fd));
	else if (req.cmd == "QUIT")
		return (this->_cmd_quit(req, fd));
    else if (req.cmd == "PART")
        return (this->_cmd_part(req, fd));
	else
		return ("Unknown command\r\n");
}

std::string	Server::getName(void) const { return (this->_name); }
std::string	Server::getPass(void) const { return (this->_pass); }
int			Server::getOnline(void) const { return (this->_online); }
int			Server::getMaxOnline(void) const { return (this->_max_online); }

std::ostream &operator<<(std::ostream &o, const Server &s)
{
	o << "========================================" << std::endl;
	o << "Server Name: " << s.getName() << std::endl;
	o << "Online: " << s.getOnline() << std::endl;
	o << "Max Online: " << s.getMaxOnline() << std::endl;
	o << "Password: " << s.getPass() << std::endl;
	o << "========================================" << std::endl;
	return (o);
}

void Server::sendMessageToChannelUsers(const std::string& channel_name, const std::string& message, int fd)
{
    int send_ret;
    std::map<std::string, Channel*>::iterator it = this->_channels.find(channel_name);
    if (it != this->_channels.end())
    {
        Channel *channel = it->second;
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
    else
    {
        std::cerr << "Channel not found: " << channel_name << std::endl;
    }
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


std::string Server::getChannelNames(void) const
{
    std::string channel_names;
    std::map<std::string, Channel*>::const_iterator it;
    for (it = this->_channels.begin(); it != this->_channels.end(); ++it)
    {
        channel_names += it->first;
        channel_names += " ";
    }
    return channel_names;
}

int	Server::_fdByNick(std::string nick)
{
	// std::map<int, Client*>::iterator	it = this->_clients.begin();

	for (std::map<int, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
	{
		if (it->second->getNick() == nick)
			return (it->first);
	}
	// not found
	return (-1);
}

std::vector<char>	Server::_splitModes(std::string modes)
{
	std::vector<char>	ret;
	for (unsigned int i = 1; i < modes.length(); i++)
		ret.push_back(modes[i]);
	return (ret);
}

// i s w o
char	Server::_validUserMode(std::vector<char> modes, bool &validMode)
{
	for (unsigned int i = 0; i < modes.size(); i++)
	{
		if (modes[i] != 'i' && modes[i] != 's' && modes[i] != 'w' && modes[i] != 'o')
		{
			validMode = false;
			return (modes[i]);
		}
	}
	return (0);
}
