
#ifndef IRCPARSER_HPP
# define IRCPARSER_HPP
# include "Request.hpp"
# include <vector>
# include <iostream>

class Request;

class IrcParser
{
	public : 
		IrcParser(void);

		std::vector<Request>		parse(std::string message);
		std::vector<std::string>	split(std::string& og_message);

	private:
		std::string	_parsePrefix(std::string message, Request& req);
		std::string	_parseCmd(std::string messsage, Request& req);
		std::string	_parseParams(std::string message, Request& req);
		std::string	_parseMiddle(std::string message, Request& req);
};

#endif
