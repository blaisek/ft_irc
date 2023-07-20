/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/10 15:06:17 by saeby             #+#    #+#             */
/*   Updated: 2023/07/20 12:14:39 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# include "irc.hpp"

class Client;
class Request;
class Channel;

class Server
{
	public:
		Server(void);
		Server(std::string name, std::string pass, std::string port, int max);
		Server(const Server& other);
		Server &operator=(const Server &other);
		~Server();

		std::string		getName(void) const;
		int				getOnline(void) const;
		int				getMaxOnline(void) const;
		std::string		getPass(void) const;
        std::string     getChannelNames(void) const;

		void			start(void);
		void			sendMessageToChannelUsers(const std::string& channelName, const std::string& message, int fd);
		void			sendToAllChannelUsers(const std::string & channel_name, const std::string& message);
		void			sendPrivateMessage(const std::string& userNickname, const std::string& message, int fd );
        void			sendJoinResponses(int fd, const std::string& nick, const std::string& user_name, const std::string& ip, const std::string& channel_name, const std::string& members, const std::string& hostname);

	private:
		std::string							_name;
		std::string							_pass;
		int									_socket;
		int									_online;
		int									_max_online;
		struct pollfd						*_poll_fds;
		std::map<int, Client *>				_clients;
		std::map<std::string, Channel *>	_channels;
		std::vector<std::string>			_nicknames;
		std::string							_oper_pass;

		void					_init_socket(std::string port);
		void					_create_client(void);
		void					_remove_client(int i);
		void					_handle_request(int i);
		std::string				_get_message(std::string nick, std::string code, std::string message);
		std::string				_reply(Request req, int fd);
		int						_fdByNick(std::string nick);
		std::vector<char>		_splitModes(std::string modes);
		char					_validUserMode(std::vector<char> modes, bool &validMode);
		char					_validChannelMode(std::vector<char> modes, bool &validMode);
		void					_sendNoticeToChannel(const std::string& target, const std::string& mes, int fd);
		void					_sendNoticeToUser(const std::string& target, const std::string& mes, int fd);
		std::string				_sendBanList(Request& req, int fd);

		// commands in srcs/commands.cpp
		std::string				_cmd_nick(Request& req, int fd);
		std::string				_cmd_pass(Request& req, int fd);
		std::string				_cmd_user(Request& req, int fd);
		std::string				_cmd_ping(Request& req, int fd);
		std::string				_cmd_join(Request& req, int fd);
		std::string				_cmd_privmsg(Request& req, int fd);
		std::string				_cmd_mode(Request& req, int fd);
		std::string				_cmd_quit(Request& req, int fd);
		std::string				_cmd_part(Request& req, int fd);
		std::string				_cmd_notice(Request& req, int fd);
		std::string				_cmd_invite(Request& req, int fd);
		std::string				_cmd_who(Request& req, int fd);
		std::string				_cmd_whois(Request& req, int fd);
		std::string				_cmd_topic(Request& req, int fd);
		std::string				_cmd_oper(Request& req, int fd);
		std::string				_cmd_kick(Request& req, int fd);
		std::string				_cmd_die(Request& req, int fd);
		std::string				_userMode(Request& req, int fd);
		std::string				_channelMode(Request& req, int fd);
};

std::ostream &operator<<(std::ostream &o, const Server &s);

#endif

