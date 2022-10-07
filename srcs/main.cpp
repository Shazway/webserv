#include "Colors.hpp"
#include "HttpRequest.hpp"
#include "Parsing.hpp"
#include "Server.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

void	initFdSet(fd_set &fdSet, std::vector<int> *sockets, std::list<int> &fds)
{
	//si sockets n'est pas NULL, mettre les sockets dans fdSet en plus des fds
}

int	running(std::vector<Server> &servers)
{
	int					num = 0; //plus grand fd en utilisation + 1;
	std::vector<int>	sockets;
	std::list<int>		fds;
	fd_set				fdread;
	fd_set				fdwrite;

	int	s;
	struct sockaddr		addr;
	int opt; //ne sert a rien, mais calme le proto de setsockopt
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		init_addr(addr, *it);
		s = socket(/*a remplir*/);
		bind(s, addr);
		listen(s, 32);
		setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	}
	if (s >= num)
		num = s + 1;
	while (true)
	{
		initFdSet(fdread, &sockets, fds);
		initFdSet(fdread, NULL, fds);
		if (select(num, &fdread, &fdwrite, 0, 0) < 1)
			continue ;
		for (std::)
	}

}

int	main(int ac, char** av)
{
	std::vector<Server> servers;
	
	// Telli : je ne sais pas si ton setup_config throw des exceptions ou quoi
	//         reorganise ce bout de main si necessaire
	if (ac == 1)
		servers = setup_config("config/default.conf");
	else if (ac == 2)		
		servers = setup_config(av[1]);
	else
	{
		std::cout << RED << "ERROR" << END << std::endl;
		std::cout << "Too many arguments" << std::endl;
		return (1);
	}
	return (running(servers));
}