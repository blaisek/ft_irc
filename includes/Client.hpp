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
		Client(int fd);
		Client(const Client& other);
		Client &operator=(const Client &other);
		~Client();

	private:
		std::string							_nick_name;
		std::string							_user_name;
		std::string							_real_name;
		int									_client_fd;
		bool								_authenticated;
		bool								_is_operator;
		const std::string					_host;
		// std::map<Channel *, bool[]>			_channels; // channels pointers with modes array for specific channel
};

#endif



