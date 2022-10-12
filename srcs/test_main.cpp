/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:33:01 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/12 00:04:41 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "Webserv.hpp"
#include "Server.hpp"


void	start(std::vector<Server>& servers)
{
	Webserv	webserv;
	int		fd_listen;
	int		ret;
	int		client;

	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		fd_listen = (*it).init_socket();
		webserv.add_server(fd_listen, (*it));
	}
	while (true)
	{
		ret = epoll_wait(webserv.getEpollfd(), webserv.events.data(), webserv.getNbEvents(), -1 );
		for (int i = 0; i < ret; i++)
		{
			for () //tester toutes les sockets en faisant un iterator sur les servers
			{
				if (webserv.events[i].data.fd ==)
				{
					//accept
				}
			}
			else
			{
				client = webserv.events[i].data.fd;
				if (webserv.events[i].events & EPOLLIN)
				{
					//clients dont on doit recv un truc
				}
				else if (webserv.events[i].events & EPOLLOUT)
				{
					//clients a qui on doit send un truc
				}
			}
		}
	}
	std::cout << "Notre webserv :" << std::endl;
	std::cout << webserv << std::endl;
}

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << RED <<
		"/!\\ Wrong number of arguments:\nTry ./webserv <path_to_config_file> /!\\"
		<< END << std::endl;
		return (1);
	}
	else
	{
		std::vector<Server> servers(parse_config(av[1]));
		start(servers);
	}
	return (0);
}