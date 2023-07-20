/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saeby <saeby>                              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 09:52:09 by saeby             #+#    #+#             */
/*   Updated: 2023/07/20 15:22:59 by saeby            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

void	error(std::string msg, int code)
{
	std::cerr << msg << std::endl;
	exit(code);
}

const std::string	timestamp(void)
{
	time_t		now = time(0);
	struct tm	tstruct;
	char		buf[80];
	tstruct = *localtime(&now);

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return (buf);
}

std::string		str_tolower(std::string str)
{
	std::string	ret;
	for (unsigned int i = 0; i < str.length(); i++)
		ret.push_back(std::tolower(str[i]));
	return (ret);
}
