/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 10:41:51 by saeby             #+#    #+#             */
/*   Updated: 2023/05/30 14:08:31 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "irc.hpp"

class Client;

class Server
{
	public:
		Server(std::string name, int max_online, std::string port, std::string password);
		~Server();

		void				start(void);
	private:
		Server(void);
		Server &operator=(const Server &other);
		Server(const Server& other);

		std::string				_name;
		std::string				_port;
		std::string				_password;
		int						_socket_fd;
		struct pollfd			*_pfds;
		int						_online_clients;
		int						_max_online_clients;
		std::map<int, Client *>	_clients;

		void					_socketInit(std::string port);
		void					_createClient(void);

};

#endif

