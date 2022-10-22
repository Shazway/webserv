/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:33:01 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/18 22:38:23 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "Webserv.hpp"
#include "Server.hpp"


/***************************************************************************/
/*

SYNTAXE HTTP REQUEST : https://devstory.net/11631/comment-afficher-les-en-tetes-http-dans-google-chrome

GET /home/index.php HTTP/1.1
Host: www.eclipse.org
Connection: keep-alive
Cache-Control: max-age=0
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 6.3; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.89 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
Cookie: un bordel pas possible*/
/***************************************************************************/

bool	complete_request(std::string str)
{
	std::vector<std::string>	v;
	size_t						bodySize = 0;
	std::string					tmp;

	ft_split(str, v, "\n");
	std::vector<std::string>::iterator	it = v.begin();
	bodySize = (*it).find("Content-Length: ");
	while (it != v.end() && !(*it).empty() && bodySize == std::string::npos)
	{
		it++;
		bodySize = (*it).find("Content-Length: ");
	}
	if (bodySize != std::string::npos)
	{
		tmp = (*it).substr(17, std::string::npos);
		bodySize=atoi(tmp.c_str());
	}
	while (it != v.end() && !(*it).empty())
		it++;
	while (it != v.end() && bodySize > (*it).size())
	{
		bodySize -= (*it).size();
		it++;
	}
	if (it == v.end())
		return (false);
	return (true);
}

void	start(std::vector<Server>& servers)
{
	Webserv	webserv(servers.size());
	int		fd_listen;
	int		ret;
	int		client;
	int		count = 0;
	char	buff[BUFFER_SIZE];
	std::string	buffer_strings[EVENT_SIZE];
	bool	found = false;

	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		fd_listen = (*it).init_socket();
		(*it).setSocket(fd_listen);
		//std::cout << "FD LISTEN: " << (*it).getSocket() << std::endl;
		try
			{webserv.setServer(count, (*it));}
		catch(const std::exception& e)
			{std::cerr<< RED << " " << fd_listen << e.what() << END << '\n';}
		webserv.add_event(fd_listen, EPOLLIN);
		std::cout << "Count: " << count << std::endl;
		count++;
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
					int client = accept(webserv.getServer(j).getSocket(), NULL, NULL);
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
		/*for (int i = 0; i < EVENT_SIZE; i++)
		{
			if (complete_request(buffer_strings[client]) || buffer_strings[client].empty())
			{
				//créeer les truc fin une réponse et tout plus tard += compliqué
			}
		}*/
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