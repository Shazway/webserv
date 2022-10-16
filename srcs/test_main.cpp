/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:33:01 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/16 23:09:35 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "Webserv.hpp"
#include "Server.hpp"

bool	complete_request(std::string str)
{
	(void)str;
	//Vérifier que la string contient une requête http complete//
	return (true);
}

void	start(std::vector<Server>& servers)
{
	Webserv	webserv(servers.size());
	int		fd_listen;
	int		ret;
	int		client;
	char	buff[BUFFER_SIZE];
	std::string	buffer_strings[EVENT_SIZE];
	bool	found = false;

	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		fd_listen = (*it).init_socket();
		(*it).setSocket(fd_listen);
		webserv.setServer(fd_listen, (*it));
		webserv.add_event(fd_listen, EPOLLIN);
	}
	while (true)
	{
		ret = epoll_wait(webserv.getEpollfd(), webserv.getEvents(), webserv.getNbEvents(), -1);
		for (int i = 0; i < ret; i++)
		{
			found = false;
			for (size_t j = 0; j < servers.size(); j++) //tester toutes les sockets en faisant un iterator sur les servers
			{
				if (webserv.getEvent(i).data.fd == webserv.getServer(j).getSocket())
				{
					found = true;
					sockaddr_in	addr;
					socklen_t	len = sizeof(addr);
					int client = accept(fd_listen, (sockaddr*)&addr, &len);
					if (client <= 0)
					{
						std::cerr << RED << "/!\\ Accept for client failed /!\\" << END << std::endl;
						continue ;
					}
					webserv.add_event(client, EPOLLIN);
				}
			}
			if (!found)
			{
				client = webserv.getEvent(i).data.fd;
				if (webserv.getEvent(i).events & EPOLLIN)
				{
					if (!complete_request(buffer_strings[client]) || buffer_strings[client].empty())
					{
						recv(client, buff, BUFFER_SIZE, 0);//errors to check
						//if return 0 close la connection
						buffer_strings[client] += buff;
					}
				}
				else if (webserv.getEvent(i).events & EPOLLOUT)
				{
					//clients a qui on doit send un truc
				}
			}
		}
		//stocker un fd max, pour opti et pas passer sur les 1000 fd
		for (int i = 0; i < EVENT_SIZE; i++)
		{
			if (complete_request(buffer_strings[client]) || buffer_strings[client].empty())
			{
				//créeer les truc fin une réponse et tout plus tard += compliqué
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