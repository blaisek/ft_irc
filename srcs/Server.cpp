/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 10:41:45 by saeby             #+#    #+#             */
/*   Updated: 2023/05/30 14:56:07 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"


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
					this->_handleRequest(i);
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

void Server::_createClient(void)
{
    struct sockaddr remote_addr;
    socklen_t addrlen;
    int new_fd;
    char buf[512];

    addrlen = sizeof(addrlen);
    new_fd = accept(this->_socket_fd, (struct sockaddr *)&remote_addr, &addrlen);
    if (new_fd == -1)
    {
        std::cerr << "ERROR: accept() error: " << strerror(errno) << std::endl;
        return;
    }
    // On crée un nouvel objet Client
    Client *client = new Client(new_fd, inet_ntoa(((struct sockaddr_in *)&remote_addr)->sin_addr));
    // On ajoute le nouveau client dans le tableau de pollfd
    this->_clients.insert(std::make_pair(new_fd,client));
    this->_addPoll(new_fd, inet_ntoa(((struct sockaddr_in *)&remote_addr)->sin_addr));

    memset(buf, 0, sizeof(buf));
    int bytesRead = recv(new_fd, buf, sizeof(buf) - 1, 0);
    if (bytesRead > 0)
    {
        std::cout << "[" << timestamp() << "]: new connection from " << inet_ntoa(((struct sockaddr_in *)&remote_addr)->sin_addr) << " on socket " << new_fd << std::endl;

        std::string message = "Welcome to the Internet Relay Network";

        std::string ret = this->_name + " " + RPL_WELCOME + " "+ message + " " + "@" + client->getHost() + "\r\n";
        if (send(new_fd, ret.c_str(), ret.length(), 0) < 0)
        {
            std::cout << "ERROR send() error: " << strerror(errno) << std::endl;
            close(new_fd);
        }
    }
}

void	Server::_addPoll(int fd, std::string ip)
{
	if (this->_online_clients == this->_max_online_clients)
	{
		this->_max_online_clients *= 2;
		this->_pfds = (struct pollfd *)realloc(this->_pfds, this->_max_online_clients);
	}
	this->_pfds[this->_online_clients].fd = fd;
	this->_pfds[this->_online_clients].events = POLLIN;
	this->_clients.insert(std::pair<int, Client *>(fd, new Client(fd, ip)));
	this->_online_clients++;
}

void	Server::_removePoll(int i)
{
	close(this->_pfds[i].fd);
	this->_pfds[i] = this->_pfds[this->_online_clients - 1];
	this->_clients.erase(this->_pfds[i].fd);
	this->_online_clients--;
}

void Server::_handleRequest(int client_index)
{
    char buffer[512]; // messages shall not exceed 512 characters following IRC protocol
    int sender_fd, n;

    sender_fd = this->_pfds[client_index].fd;
    n = recv(sender_fd, buffer, sizeof(buffer), 0); // N is the number of bytes received

    if (n <= 0)
    {
        close(sender_fd);
        this->_removePoll(client_index);
    }
    else
    {
        // REQUEST HANDLING AND PARSING HAPPENS HERE
        Client* client = this->_clients[sender_fd];
        std::istringstream iss(buffer);
        std::string command;
        while (iss >> command)
        {
            if (command == "NICK")
            {
                std::string nickname;
                if (iss >> nickname)
                {
                    // Stocker le nickname dans l'objet Client
                    client->setNickName(nickname);
                    std::string message = "Your Nickname has been set to " + nickname + " !" + "\r\n";
                     send(sender_fd, message.c_str(), message.length(), 0);
                }
            }
            else if (command == "USER")
            {
                std::string user;
                if (iss >> user)
                {
                    // Stocker le user dans l'objet Client
                    client->setUserName(user);
                    std::string message = "Your Username has been set to " + user + " !" + "\r\n";
                     send(sender_fd, message.c_str(), message.length(), 0);
                }
            }
        }
    }
    memset(&buffer, 0, sizeof(buffer));
}

void Server::_createChannel(std::string channel_name)
{
    Channel *channel = new Channel(channel_name);
    this->_channels.insert(std::make_pair(channel_name, channel));
}
