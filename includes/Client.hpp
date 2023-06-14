/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 13:58:06 by saeby             #+#    #+#             */
/*   Updated: 2023/05/30 14:05:58 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP
# include "irc.hpp"

class Client
{
	public:
		Client(void);
		Client(int fd, std::string host);
		Client(const Client& other);
		Client &operator=(const Client &other);
        std::string getNickName(void) const;
        std::string getUserName(void) const;
        std::string getRealName(void) const;
        std::string getHost(void) const;
        int getClientFd(void) const;
        void setNickName(std::string nick_name);
        void setUserName(std::string user_name);
        void setRealName(std::string real_name);
        void setHost(std::string host);
		~Client();

	private:
		std::string							_nick_name;
		std::string							_user_name;
		std::string							_real_name;
        std::map <int, Client *>			_clients;
		int									_client_fd;
		bool								_authenticated;
		bool								_is_operator;
		std::string					        _host;

        void insert(std::pair<int, Client *>);
		// std::map<Channel *, bool[]>			_channels; // channels pointers with modes array for specific channel
};

#endif



