/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Blaze <btchiman@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 16:02:39 by Blaze             #+#    #+#             */
/*   Updated: 2023/07/19 16:11:17 by Blaze            ###    42Lausanne.ch    */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string Server::_cmd_kick(Request& req, int fd)
{
	std::string channel_name = req.params[0];
	std::string target = req.params[1];
	std::string nick = this->_clients[fd]->getNick();



}
