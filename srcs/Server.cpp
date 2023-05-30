/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 10:41:45 by saeby             #+#    #+#             */
/*   Updated: 2023/05/30 14:17:20 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string name, int max_online, std::string port, std::string password)
{
	this->_name = name;
	this->_max_online_clients = max_online + 1;
	this->_password = password;
	this->_online_clients = 0;
	this->_pfds = new struct pollfd[max_online];
	this->_socketInit(port);
	this->_pfds[0].fd = this->_socket_fd;
	this->_pfds[0].events = POLLIN;
	this->_online_clients++;
}

// Default constructor
Server::Server(void) {}

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
	// destroy saved clients & channels in here
	return ;
}

// Public member functions

void	Server::start(void)
{
	while(1)
	{
		int	pollcount = poll(this->_pfds, this->_online_clients, 1);
		if (pollcount < 0)
			error(strerror(errno), -1);
		for (int i = 0; i < this->_online_clients; i++)
		{
			if (this->_pfds[i].revents & POLLIN)
			{
				if (this->_pfds[i].fd == this->_socket_fd)
					this->_createClient();
				else
					std::cout << "Handle request" << std::endl;
			}
		}
	}
}

void	Server::_socketInit(std::string port)
{
	int 			status;
	const int		enabled = 1;
	struct addrinfo	serv, *serverinfo;

	memset(&serv, 0, sizeof(serv));
	serv.ai_family = AF_INET; // IPV4
	serv.ai_socktype = SOCK_STREAM;
	status = getaddrinfo("0.0.0.0", port.c_str(), &serv, &serverinfo);
	if (status < 0)
		error("ERROR getaddrinfo", 1);
	this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socket_fd < 0)
		error("ERROR creating socket", 1);
	setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enabled, sizeof(enabled));
	if (bind(this->_socket_fd, serverinfo->ai_addr, serverinfo->ai_addrlen) < 0)
		error("ERROR binding socket", 1);
	if (listen(this->_socket_fd, this->_max_online_clients) < 0)
		error("ERROR listening on socket", 1);
}

void	Server::_createClient(void)
{
	struct sockaddr	remote_addr;
	socklen_t		addrlen;
	int				new_fd;

	addrlen = sizeof(addrlen);
	new_fd = accept(this->_socket_fd, (struct sockaddr *)&remote_addr, &addrlen);
	if (new_fd == -1)
		std::cerr << "ERROR: accept() error: " << strerror(errno) << std::endl; // should not kill the server;
	else
	{
		// Add new_fd to _pfds[]
		// check if max online is reached
		// 		if reached, realloc to accomodate twice this number
		// insert a std::pair<new_fd, Client *> into _clients
		// Increase online client count
		// All of the above can be done in a separate function

		// send a welcome message back to the new client
		std::string message = "Welcome to our ft_irc server";
		if (send(new_fd, message.c_str(), message.length(), 0) < 0)
			std::cerr << "ERROR: send() error: " << strerror(errno) << std::endl;
		std::cout << "[" << timestamp() << "]: new connection from " << inet_ntoa(((struct sockaddr_in*)&remote_addr)->sin_addr) << " on socket " << new_fd << std::endl;
	}
}
