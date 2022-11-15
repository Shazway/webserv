/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:33:01 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/15 21:34:19 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "Parsing.hpp"
#include "Webserv.hpp"
#include <csignal>

Webserv webserv;

void signalHandler(int signum)
{
	if (signum == SIGINT)
	{
		for (std::map<int, Upload>::iterator it = webserv.uploads.begin(); it != webserv.uploads.end(); it++)
			if ((*it).second._file.is_open())
				(*it).second.closeFile();
		std::cout << MAGENTA << "Terminating server" << END << std::endl;
		/*sleep(1);
		std::cout << MAGENTA << ". " << std::flush;
		sleep(1);
		std::cout << ". " << std::flush;
		sleep(1);
		std::cout << "." << std::endl;
		std::cout << "Done !" << END <<std::endl;*/
		exit(0);
	}
	else
	{
		std::cout << BLINK_CYAN << "This signal does nothing ! <(^OO^)>" << END << std::endl;
	}
}

void	separate_lines(std::vector<std::string> &lines, std::string content)
{
	size_t	n;
	while (!content.empty())
	{
		n = find_first_line(content, 1);
		lines.push_back(content.substr(0, n));
		if (n != std::string::npos)
			content = content.substr(n, std::string::npos);
		else
			content.clear();
	}
}

bool	is_empty(std::string str)
{
	if (str == "\n" || str == "\r\n" || str == "\r" || str.empty())
		return (true);
	return (false);
}

bool	complete_request(std::string str, size_t maxBodySize)
{
	std::vector<std::string>	v;
	size_t						bodySize = 0;
	size_t						i = 0;
	std::string					tmp;

	if (str.empty())
		return (false);
	separate_lines(v, str);
	v_str_it	it = v.begin();

	if (v.empty())
		return (false);
	// Probleme avec la size du body ?
	//Renvoie false alors que c'est complet
	i = std::string::npos;
	while (it != v.end() && !is_empty(*it) && i == std::string::npos)
	{
		i = (*it).find("Content-Length: ");
		if (i == std::string::npos)
			it++;
		if (it == v.end())
			return (false);
	}
	if (i != std::string::npos)
	{
		i = ((*it).find("\n") < (*it).find("\r") ? (*it).find("\n") : (*it).find("\r"));
		tmp = (*it).substr(16, i);
		bodySize=atoi(tmp.c_str());

		if (bodySize > maxBodySize) //si bodySize est plus grand que la limite, true pour code d'erreur apres
			return (true);
	}
	while (it != v.end() && !is_empty((*it)))
		it++;
	if (bodySize == 0)
		return (true);
	it++;
	while (it != v.end() && bodySize > (*it).size())
	{
		bodySize -= ((*it).size());
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
		if (webserv.getEvent((*it).first).events & EPOLLOUT)
		{
			send((*it).first, (*it).second.c_str(), (*it).second.size(), MSG_NOSIGNAL);
			std::cout << MAGENTA << "Sending to " << (*it).first << ": [" << (*it).second << "]" << END << std::endl;
		}
	}
	answers.clear();
}

void	closeFd(int fd)
{
	close(fd);
	webserv.remove_event(fd);
}

void	start(std::vector<Server>& servers)
{
	int		fd_listen;
	int		fdMax = 0;
	int		ret;
	int		client;
	int		read_char;
	int		count = 0;
	char	buff[BUFFER_SIZE + 1];
	std::string	buffer_strings[EVENT_SIZE];
	bool	found = false;


	std::signal(SIGINT, signalHandler);
	webserv.allocating(servers.size());
	webserv.setMaxEvent(0);
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		fd_listen = (*it).init_socket();
		if (fd_listen > 0)
		{
			(*it).setSocket(fd_listen);
			if (fd_listen > fdMax)
			{
				fdMax = fd_listen;
				webserv.setMaxEvent(fdMax);
			}
			//std::cout << "FD LISTEN: " << (*it).getSocket() << std::endl;
			try
				{webserv.setServer(count, (*it));}
			catch(const std::exception& e)
				{std::cerr<< RED << " " << fd_listen << e.what() << END << '\n';}
			webserv.add_event(fd_listen, EPOLLIN);
			count++;
		}
		else
		{
			//std::cout << fd_listen << std::endl;
			exit(fd_listen);
		}
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
					//std::cout << RED << "Accepté pour FD: " << client<<END << std::endl;
					if (client <= 0)
					{
						std::cerr << RED << "/!\\ Accept for client failed /!\\" << END << std::endl;
						continue ;
					}
					if (client > fdMax)
					{
						fdMax = client;
						webserv.setMaxEvent(fdMax);
					}
					webserv.client_serv[client] = webserv.getServer(j);
					webserv.add_event(client, EPOLLIN | EPOLLOUT);
				}
			}
			if (!found)
			{
				client = webserv.getEvent(i).data.fd;
				if (webserv.getEvent(i).events & EPOLLIN)
				{
					read_char = 1;
					while (!complete_request(buffer_strings[client], webserv.client_serv[client].getBody()))
					{
						memset(buff, 0, sizeof(char) * (BUFFER_SIZE + 1));
						read_char = recv(client, buff, BUFFER_SIZE, MSG_DONTWAIT);//errors to check
						//if return 0 close la connection
						if (read_char <= 0)
						{
							if (read_char < 0)
								buffer_strings[client].clear();
							std::cout << "Read a return: " << read_char << std::endl;
							closeFd(client);
							webserv.requests.erase(client);
						}
						if (read_char <= 0)
							break;
						buffer_strings[client].append(buff, read_char);
					}
					//std::cout << RED << buffer_strings[client] << END << std::endl;
					found = true;
				}
			}
		}
		//stocker un fd max, pour opti et pas passer sur les 1000 fd
		for (int i = 0; i <= fdMax ; i++)
		{
			/*if (!buffer_strings[client].empty())
				std::cout << GREEN << complete_request(buffer_strings[client]) << buffer_strings[client] << END << std::endl;*/
			//pour une raison inconnue, complete_request renvoie false au lieu de true sur les POST
			if (complete_request(buffer_strings[i], webserv.client_serv[i].getBody()))
			{
				std::cout << "BUFFER = " << buffer_strings[i] << std::endl;
				//std::cout << "Buffer " << i << "=" << buffer_strings[i] << "." << std::endl;
				HttpRequest	tmp_request(webserv.client_serv[i]);
				try
				{
					if (parsingRequest(tmp_request, buffer_strings[i]))
					{
						buffer_strings[i].clear();
					}
					else
					{
						//std::cout << RED << tmp_request << END << std::endl;
						webserv.requests.insert(std::pair<int, HttpRequest>(i, tmp_request));
					}
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
				}
			}
		}
		answers_gen(webserv.requests, webserv.answers, webserv.uploads, webserv.client_serv);
		if (!webserv.answers.empty())
			send_answers(webserv.answers);
		/*for (int i = 0; i < EVENT_SIZE; i++)
			buffer_strings[i].clear();*/ // DECOMMENTER POUR REPARER LES DOUBLONS !!!!
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
		parse_config(av[1], webserv.servs);
		start(webserv.servs);
	}
	return (0);
}