/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:33:01 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/05 15:08:25 by tmoragli         ###   ########.fr       */
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

Webserv	webserv;

std::string	itoa(long nb)
{
	std::stringstream	ss;

	ss << nb;
	return (ss.str());
}

bool	complete_request(std::string str, size_t maxBodySize)
{
	std::vector<std::string>	v;
	size_t						bodySize = 0;
	size_t						i = 0;
	std::string					tmp;

	ft_split(str, v, "\r\n");
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
		if (bodySize > maxBodySize) //si bodySize est plus grand que la limite, true pour code d'erreur apres
			return (true);
	}
	while (it != v.end() && !(*it).empty())
		it++;
	while (it != v.end() && bodySize > (*it).size())
	{
		bodySize -= (*it).size() + 1;
		it++;
	}
	if (it == v.end())
		return (false);
	return (true);
}

void	send_answers(std::map<int, std::string>& answers)
{
	for (std::map<int, std::string>::iterator it = answers.begin(); it != answers.end(); it++)
		send((*it).first, (*it).second.c_str(), (*it).second.size(), MSG_NOSIGNAL);
}

void	generate_ok(int fd, std::map<int, std::string>& answers, std::ifstream& file)
{
	std::string content;
	std::string	line;

	//ici, check allowedmethod et faire une erreur adaptee
	answers[fd] = "HTTP/1.1 200 OK\n";
	content.clear();
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			content += line;
			content += "\n";
		}
	}
	answers[fd] += "Content length: ";
	answers[fd] += itoa((long)content.length());
	answers[fd] += "\n\n";
	answers[fd] += content;
}

void	gen_get(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answers)
{
	//GET peut avoir des variables dans la query
	if ((*it).second._serv.checkAllowedMethods("GET", (*it).second.getPath()))
	{
		std::ifstream file;
		std::string abs_path = (*it).second._serv.getRootPath() + (*it).second.getPath();

		//std::cout << YELLOW << "First open: [" << abs_path << "]" << END << std::endl;
		file.open(abs_path.c_str());
		if (file.is_open() && file.peek() == std::ifstream::traits_type::eof())
		{
			file.close();
			abs_path = (*it).second._serv.getRootPath() + (*it).second._serv.html.getClosestDirectory((*it).second.getPath()).second;
			//std::cout << YELLOW << "[" << abs_path << "]" << END << std::endl;
			file.open(abs_path.c_str());
			if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof())
				answers[(*it).first] = "HTTP/1.1 404 Not found 1\n\n";
			else
				generate_ok((*it).first, answers, file);
		}
		else if (file.is_open())
			generate_ok((*it).first, answers, file);
		else
			answers[(*it).first] = "HTTP/1.1 404 Not found\n\n";
		file.close();
	}
	else
		answers[(*it).first] = "HTTP/1.1 405 Method not allowed\n\n";
	//Header à rajouter plus tard \n \n
}

void	gen_post(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answers)
{
	v_str	stock_var;
	
	/*if ((*it).second.getContentType().find("multipart/form-data") != std::string::npos)
		std::cout << "Hello file" << std::endl;*/
	if ((*it).second._serv.checkAllowedMethods("POST", (*it).second.getPath()))
	{
		std::ifstream	file;
		if ((*it).second.getContentType().find("multipart/form-data") != std::string::npos)
			generate_ok((*it).first, answers, file);

		/*if ((*it).second.getContentType().find("application/x-www-form-urlencoded") != std::string::npos)
		{
			std::string abs_path = (*it).second.getPath();

			
			//	ft_split((*it).second.getBody(), stock_var, "&");
			std::cout << "Hello form" << std::endl;
			//std::cout << YELLOW << "First open: [" << abs_path << "]" << END << std::endl;
			file.open(abs_path.c_str());
			if (file.is_open() && file.peek() == std::ifstream::traits_type::eof())
			{
				file.close();
				abs_path = (*it).second._serv.getRootPath() + (*it).second._serv.html.getClosestDirectory((*it).second.getPath()).second;
				//std::cout << YELLOW << "[" << abs_path << "]" << END << std::endl;
				file.open(abs_path.c_str());
				if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof())
					answers[(*it).first] = "HTTP/1.1 404 Not found 1\n\n";
				else
					create_temp((*it).first, answers, file);
			}
			else if (file.is_open())
				generate_ok((*it).first, answers, file);
			else
				answers[(*it).first] = "HTTP/1.1 404 Not found\n\n";
			file.close();
		}*/
	}
	else
		answers[(*it).first] = "HTTP/1.1 405 Method not allowed\n\n";
}

void	answers_gen(std::map<int, HttpRequest>& requests, std::map<int, std::string>& answers)
{
	for (std::map<int, HttpRequest>::iterator it = requests.begin(); it != requests.end(); it++)
	{
		if ((*it).second.getMethod() == "GET")
			gen_get(it, answers);
		else if ((*it).second.getMethod() == "POST")
		{
			upload((*it).second.getBody());
			answers[(*it).first] = "HTTP/1.1 100 OK\n";
			//gen_post(it, answers);
		}
		//std::cout << BLUE << "[" <<(*it).second.getMethod()<<"]" << " Body: " << ((*it).second.getBody()) << END << std::endl;
	}
}

void	closeFd(int fd)
{
	close(fd);
	webserv.remove_event(fd);
}

void	start(std::vector<Server>& servers)
{
	int		fd_listen;
	int		fdMax;
	int		ret;
	int		client;
	int		read_char;
	int		count = 0;
	char	buff[BUFFER_SIZE + 1];
	std::map<int, HttpRequest>	requests;
	std::map<int, std::string> answers;
	std::string	buffer_strings[EVENT_SIZE];
	bool	found = false;
	std::map<int, Server>		client_serv;

	webserv.allocating(servers.size());
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		fd_listen = (*it).init_socket();
		(*it).setSocket(fd_listen);
		if (fd_listen > fdMax)
			fdMax = fd_listen;
		//std::cout << "FD LISTEN: " << (*it).getSocket() << std::endl;
		try
			{webserv.setServer(count, (*it));}
		catch(const std::exception& e)
			{std::cerr<< RED << " " << fd_listen << e.what() << END << '\n';}
		webserv.add_event(fd_listen, EPOLLIN);
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
					if (client > fdMax)
						fdMax = client;
					client_serv[client] = webserv.getServer(j);
					webserv.add_event(client, EPOLLIN);
				}
			}
			if (!found)
			{
				client = webserv.getEvent(i).data.fd;
				if (webserv.getEvent(i).events & EPOLLIN)
				{
				//std::cout << "here" << std::endl;
					if (!complete_request(buffer_strings[client], client_serv[client].getBody()) || buffer_strings[client].empty())
					{
						memset(buff, 0, BUFFER_SIZE + 1);
						read_char = recv(client, buff, BUFFER_SIZE, 0);//errors to check
						//if return 0 close la connection
						if (read_char <= 0)
						{
							closeFd(client);
							requests.erase(client);
						}
						buffer_strings[client] += buff;
					}
				}
				else if (webserv.getEvent(i).events & EPOLLOUT)
				{
					//clients a qui on doit send un truc
					if (!answers[client].empty())
						send(client, answers[client].c_str(), answers[client].size(), MSG_NOSIGNAL);
					answers.erase(client);
				}
			}
		}
		//stocker un fd max, pour opti et pas passer sur les 1000 fd
		for (int i = 0; i <= fdMax ; i++)
		{
			/*if (!buffer_strings[client].empty())
				std::cout << GREEN << complete_request(buffer_strings[client]) << buffer_strings[client] << END << std::endl;*/
			//pour une raison inconnue, complete_request renvoie false au lieu de true sur les POST
			if (!buffer_strings[i].empty() && complete_request(buffer_strings[i], client_serv[client].getBody()))
			{
				HttpRequest	tmp_request(client_serv[i]);
				try
				{
					parsingRequest(tmp_request, buffer_strings[i]);
					buffer_strings[i].clear();
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
				}
				//std::cout << RED << tmp_request << END << std::endl;
				requests.insert(std::pair<int, HttpRequest>(i, tmp_request));
			}
		}
		answers_gen(requests, answers);
		send_answers(answers);
		answers.clear();
		requests.clear();
		//for (int i = 0; i < EVENT_SIZE; i++)
		//	buffer_strings[i].clear();
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