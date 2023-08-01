
#include "irc.hpp"

int	main(int ac, char **av)
{
	if (ac != 3)
		error("Usage: ./ircserv <port> <password>", 1);

	try
	{
		Server srv("ft_irc", av[2], av[1], 10);
		srv.start();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	return (0);
}
