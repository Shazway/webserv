#include "Colors.hpp"
#include "HttpRequest.hpp"
#include "Parsing.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <fcntl.h>

//pour start

//extraire les donnees pertinentes de Server pour les mettre dans sockaddr
void	init_addr(struct sockaddr *addr, Server serveur);

void	initFdSet(fd_set &fdSet, std::vector<int> *sockets, std::list<int> &fds)
{
	//si sockets n'est pas NULL, mettre les sockets dans fdSet en plus des fds
}

int	running(std::vector<Server> &servers)
{
	int					num = 0; //plus grand fd en utilisation + 1;
	std::vector<Socket>	sockets;
	std::list<Socket>		fds;
	Socket				socketMaker;
	fd_set				fdread;
	fd_set				fdwrite;
	HttpRequest		tmp_request;
	std::map<int, HttpRequest> requests;
	std::string			buffer;

	int opt; //ne sert a rien, mais calme le proto de setsockopt
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		init_addr(socketMaker.getAddr(), *it);
		socketMaker.setS(socket(/*a remplir*/));
		bind(socketMaker.getS(), addr);
		listen(socketMaker.getS(), 32);
		setsockopt(socketMaker.getS(), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
		sockets.push_back(socketMaker);
	}
	if (s >= num)
		num = s + 1;
	while (true)
	{
		initFdSet(fdread, &sockets, fds);
		initFdSet(fdwrite, NULL, fds);
		if (select(num, &fdread, &fdwrite, 0, 0) < 1)
			continue ;
		//for des sockets
		for (std::vector<Socket>::iterator it = sockets.begin(); it != sockets.end(); it++)
		{
			if (FD_ISSET((*it).getS(), fdread))
			{
				socketMaker.setS(accept((*it).getS(), (*it).getAddr(), sizeof(struct sockaddr)));
				fcntl(socketMaker.getS(), O_NONBLOCK);
				//faire un truc avec socketMaker._addr
				fds.push_back(socketMaker);
				if (socketMaker.getS() >= num)
					num = socketMaker.getS() + 1;
			}
		}
		//for des fds
		for (std::list<Socket>::iterator it = fds.begin(); it != fds.end(); it++)
		{
			if (FD_ISSET((*it).getS(), fdread))
			{
				if (recv((*it).getS(), buffer, 2048, 0) <= 0)
				{
					close ((*it).getS());
					fds.erase((*it).getS());
				}
				else if (!buffer.empty())
				{
					do
					{
						try
						{
							ParsingRequest(tmp_request, buffer);
						}
						catch(const std::exception& e)
						{
							std::cerr << RED << e.what()<< END << std::endl;
						}
					}
					while (tmp_request.getPartiallyCompleted());
					requests[(*it).getS()] = tmp_request;
				}
			}
			if (FD_ISSET((*it).getS(), fdwrite))
			{
				
			}
		}
	}
	return 0;
}

int	main(int ac, char** av)
{
	std::vector<Server> servers;
	

	if (ac == 1)
		servers = parse_config("config/default.conf");
	else if (ac == 2)		
		servers = parse_config(av[1]);
	else
	{
		std::cout << RED << "ERROR" << END << std::endl;
		std::cout << "Too many arguments" << std::endl;
		return (1);
	}
	return (running(servers));
}