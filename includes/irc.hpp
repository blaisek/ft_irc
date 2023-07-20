/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 09:52:46 by saeby             #+#    #+#             */
/*   Updated: 2023/07/20 18:15:56 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP
# include "errors.hpp"
# include "responses.hpp"
# include <iostream>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <string>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <poll.h>
# include <map>
# include <vector>
# include <list>
# include <algorithm>
# include "Server.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "IrcParser.hpp"
# include "Request.hpp"
# include "Channel.hpp"

// | ================== Defines ================= |

# define HOSTNAME "irc.42lausanne.ch"

// | ============== srcs/utils.cpp ============== |
void						error(std::string msg, int code);
const						std::string	timestamp(void);
std::string					str_tolower(std::string str);

#endif