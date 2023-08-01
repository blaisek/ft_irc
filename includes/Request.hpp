
#ifndef REQUEST_HPP
# define REQUEST_HPP
# include <iostream>
# include <vector>

class Request
{
	public:
		Request(void);

		bool						valid;
		std::string					og;
		std::string					prefix;
		std::string					cmd;
		std::vector<std::string>	params;
		int							param_count;
		std::string					middle;
		std::string					trailing;
};

std::ostream	&operator<<(std::ostream& o, const Request& req);

#endif
