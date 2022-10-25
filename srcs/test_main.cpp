/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:33:01 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/25 22:12:24 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"
#include "Webserv.hpp"
#include "Server.hpp"
#include <stdlib.h>
#include <sstream>
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

std::string	itoa(long nb)
{
	std::stringstream	ss;

	ss << nb;
	return (ss.str());
}

bool	complete_request(std::string str)
{
	std::vector<std::string>	v;
	size_t						bodySize = 0;
	size_t						i = 0;
	std::string					tmp;

	ft_split(str, v, "\n");
	std::vector<std::string>::iterator	it = v.begin();
	i = (*it).find("Content-Length: ");
	while (it != v.end() && !(*it).empty() && i == std::string::npos)
	{
		it++;
		if (it == v.end())
			return (0);
		i = (*it).find("Content-Length: ");
	}
	if (i != std::string::npos)
	{
		tmp = (*it).substr(16, std::string::npos);
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

void	send_answers(std::map<int, std::string>& answers)
{
	for (std::map<int, std::string>::iterator it = answers.begin(); it != answers.end(); it++)
	{
		send((*it).first, (*it).second.c_str(), (*it).second.size(), MSG_NOSIGNAL);
		std::cout << (*it).second << std::endl;
	}
}
void	answers_gen(std::map<int, HttpRequest>& requests, std::map<int, std::string>& answers)
{
	std::string	line;
	std::string content;

	for (std::map<int, HttpRequest>::iterator it = requests.begin(); it != requests.end(); it++)
	{
		if ((*it).second.getMethod() == "GET")
		{
			if ((*it).second._serv.checkAllowedMethods("GET", (*it).second.getPath()))
			{
				std::ifstream file;
			
				file.open((*it).second.getPath().c_str());
				//std::cout << YELLOW << "[" << (*it).second << "]" << END << std::endl;
				if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof())
					answers[(*it).first] = "HTTP/1.1 404 Not found\n\n";
				else
				{
					answers[(*it).first] = "HTTP/1.1 200 OK\n";
					content.clear();
					while (std::getline(file, line))
						content += line;
					answers[(*it).first] += "Content length: ";
					answers[(*it).first] += itoa((long)content.length());
					answers[(*it).first] += "\n\n";
					answers[(*it).first] += content;
				}
			}
			else
				answers[(*it).first] = "HTTP/1.1 405 Method not allowed\n\n";
			//Header à rajouter plus tard \n \n
		}
	}
}

void	start(std::vector<Server>& servers)
{
	Webserv	webserv(servers.size());
	int		fd_listen;
	int		ret;
	int		client;
	int		count = 0;
	char	buff[BUFFER_SIZE];
	std::map<int, HttpRequest>	requests;
	std::map<int, std::string> answers;
	std::string	buffer_strings[EVENT_SIZE];
	bool	found = false;
	std::map<int, Server>		client_serv;

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
					client_serv[client] = webserv.getServer(j);
					webserv.add_event(client, EPOLLIN);
				}
			}
			if (!found)
			{
				std::cout << "here" << std::endl;
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
					/*if (!answers.empty())
					{
						// if return <= 0 close la co*/
					send(client, answers[client].c_str(), answers[client].size(), MSG_NOSIGNAL);
					
				}
			}
		}
		//stocker un fd max, pour opti et pas passer sur les 1000 fd
		for (int i = 0; i < EVENT_SIZE; i++)
		{
			if (complete_request(buffer_strings[client]) || buffer_strings[client].empty())
			{
				HttpRequest	tmp_request(client_serv[client]);
				try
				{
					parsingRequest(tmp_request, buffer_strings[client]);
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
				}
				std::cout << RED << tmp_request << END << std::endl;
				requests.insert(std::pair<int, HttpRequest>(client, tmp_request));
			}
		}
		answers_gen(requests, answers);
		send_answers(answers);
	}
	/*std::cout << "Notre webserv :" << std::endl;
	std::cout << webserv << std::endl;*/
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