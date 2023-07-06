/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 16:18:57 by saeby             #+#    #+#             */
/*   Updated: 2023/07/06 14:54:15 by saeby            ###   ########.fr       */
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

		int							getFd(void) const;
		bool						isAuth(void) const;
		std::string					getNick(void) const;
		std::string					getUser(void) const;
		std::string					getIdentity(void) const;
		std::string					getHost(void) const;
		bool						getReg(void) const;
		std::string					getFullName(void) const;
		std::string					getModes(void) const;
		bool						isOp(void) const;

		void						setAuth(bool auth);
		void						setNick(std::string nick);
		void						setUser(std::string user);
		void						setIdentity(std::string identity);
		void						setHost(std::string host);
		void						setReg(bool reg);
		void						setFullName(std::string fullname);
		void						setMode(char mode, bool setMode);

	private:
		int							_fd;
		bool						_auth;
		std::string					_nick;
		std::string					_user;
		std::string					_identity;
		std::string					_host;
		bool						_reg;
		std::string					_fullName;
		bool						_srvOperator;
		std::map<char, bool>		_modes;
};

std::ostream	&operator<<(std::ostream const &o, Client &c);

#endif

