/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:33:01 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/08 23:01:15 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <vector>
#include "Parsing.hpp"
#include "Webserv.hpp"

Webserv webserv;

std::string	itoa(long nb)
{
	std::stringstream	ss;

	ss << nb;
	return (ss.str());
}

void	separate_lines(v_str &lines, std::string content)
{
	while (!content.empty())
	{
		lines.push_back(content.substr(0, find_first_line(content, 1) - 1));
		content = content.substr(find_first_line(content, 1));
	}
}

bool	is_empty(std::string str)
{
	if (str == "\n" || str == "\r\n")
		return (true);
	return (false);
}

bool	complete_request(std::string str, size_t maxBodySize)
{
	v_str						v;
	size_t						bodySize = 0;
	size_t						i = 0;
	std::string					tmp;

	std::cout << "Start[" << RED << str << END << "]end" << std::endl;
	separate_lines(v, str);
	display_v_str(v);
	std::cout << "Size: " << v.size() << std::endl;
	v_str_it	it = v.begin();

	if (v.empty()) // sinon ça segfault ??
		return (0);
	// Probleme avec la size du body ?
	//Renvoie false alors que c'est complet
	i = (*it).find("Content-Length: ");
	while (it != v.end() && !is_empty(*it) && i == std::string::npos)
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
		std::cout << GREEN << "ICI bodysize " << bodySize << " maxbodysize" << maxBodySize << std::endl;

		if (bodySize > maxBodySize) //si bodySize est plus grand que la limite, true pour code d'erreur apres
			return (true);
	}
	while (it != v.end() && !is_empty((*it)))
		it++;
	//std::cout << GREEN << "ICI line " << *(++it) << std::endl;
	it++;
	while (it != v.end() && bodySize > (*it).size())
	{
		std::cout << "reste a recuperer " << bodySize << " et actuellement " << (*it).size() << std::endl;
		std::cout << (*it).size() << std::endl;
		bodySize -= ((*it).size());
		it++;
	}
	//std::cout << GREEN << "ICI derniere ligne " << *(--it) << std::endl;
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
			//std::cout << YELLOW << "Sending to " << (*it).first << ": [" << (*it).second << "]" << END << std::endl;
			answers.erase(it);
		}
	}
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
	//std::cout << "FD: [" << fd << "]" << std::endl;
	//std::cout << "CONTENT: [" << content << "]" << std::endl;
	content = content.substr(0, content.size() - 1);
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

void	gen_post(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answers, std::map<int, Upload> uploads)
{
	int	fd = (*it).first;
	HttpRequest request = (*it).second;

	if (request._serv.checkAllowedMethods("POST", request.getPath()))
	{
		if ((*it).second.getContentType().find("application/x-www-form-urlencoded") != std::string::npos)// Faut download
		{
			Upload	up; //Crée une instance de upload pour l'ajouter si il existe pas dans map

			if (uploads.find(fd) == uploads.end()) //Il existe pas
			{
				if (upload(up, request.getBody()) == COMPLETE) //Le body contient le delimiteur de fin alors upload a renvoyé 1
					answers[(*it).first] = "HTTP/1.1 200 OK\n";// Pas besoin de l'ajouter a la map puisqu'il est entier
				else
				{
					answers[(*it).first] = "HTTP/1.1 206 Partial Content\n";//Il n'y a pas de delimiteur de fin
					uploads[fd] = up; //Ajout de up a la map
				}
			}
			else//Il existe
			{
				if (upload(uploads[fd], request.getBody()) == COMPLETE)// On vient de recevoir la fin
				{
					answers[(*it).first] = "HTTP/1.1 200 OK\n";
					uploads.erase(uploads.find(fd));//On l'efface de la map car on a fini de download le fichier
				}
				else
					answers[(*it).first] = "HTTP/1.1 206 Partial Content\n";//upload a renvoyé INCOMPLETE, on demande la suite du body
			}
		}
	}
}

void	answers_gen(std::map<int, HttpRequest>& requests, std::map<int, std::string>& answers, std::map<int, Upload>& uploads)
{
	//gerer ici si le body est trop grand
	for (std::map<int, HttpRequest>::iterator it = requests.begin(); it != requests.end(); it++)
	{
		//std::cout << RED << "here" << END << std::endl;
		if ((*it).second.getMethod() == "GET")
			gen_get(it, answers);
		else if ((*it).second.getMethod() == "POST")
			gen_post(it, answers, uploads);
		//std::cout << BLUE << "[" <<(*it).second.getMethod()<<"]" << " Body: " << ((*it).second.getBody()) << END << std::endl;
		requests.erase(it);
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
	int		fdMax = 0;
	int		ret;
	int		client;
	int		read_char;
	int		count = 0;
	char	buff[BUFFER_SIZE + 1];
	std::map<int, HttpRequest>	requests;
	std::map<int, std::string>	answers;
	std::map<int, Upload>		uploads;
	std::string	buffer_strings[EVENT_SIZE];
	bool	found = false;
	std::map<int, Server>		client_serv;

	webserv.allocating(servers.size());
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		fd_listen = (*it).init_socket();
		if (fd_listen > 0)
		{
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
		else
		{
			std::cout << fd_listen << std::endl;
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
					std::cout << RED << "Accepté pour FD: " << client<<END << std::endl;
					if (client <= 0)
					{
						std::cerr << RED << "/!\\ Accept for client failed /!\\" << END << std::endl;
						continue ;
					}
					if (client > fdMax)
						fdMax = client;
					client_serv[client] = webserv.getServer(j);
					webserv.add_event(client, EPOLLIN | EPOLLOUT);
				}
			}
			if (!found)
			{
				client = webserv.getEvent(i).data.fd;
				if (webserv.getEvent(i).events & EPOLLIN)
				{
					if (!complete_request(buffer_strings[client], client_serv[client].getBody()) || buffer_strings[client].empty())
					{
						memset(buff, 0, BUFFER_SIZE + 1);
						read_char = recv(client, buff, BUFFER_SIZE, 0);//errors to check
						//std::cout << BLUE << "FD: " << client << "asks for Content: \n" << buff <<END<< std::endl;
						//if return 0 close la connection
						if (read_char <= 0)
						{
							closeFd(client);
							requests.erase(client);
						}
						//if (read_char > BUFFER_SIZE)
						//std::cout << "C'est la merde ? " << read_char << std::endl;
						buffer_strings[client] += buff;
					}
					else
						std::cout << "Requete complete" << std::endl;
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
				//std::cout << "Buffer " << i << "=" << buffer_strings[i] << "." << std::endl;
				HttpRequest	tmp_request(client_serv[i]);
				try
				{
					parsingRequest(tmp_request, buffer_strings[i]);
					//std::cout << "parsing a rendu " << res << std::endl;
					//buffer_strings[i].clear();
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
				}
				//std::cout << RED << tmp_request << END << std::endl;
				requests.insert(std::pair<int, HttpRequest>(i, tmp_request));
			}
		}
		answers_gen(requests, answers, uploads);
		send_answers(answers);
		requests.clear();
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
		std::vector<Server> servers(parse_config(av[1]));
		start(servers);
	}
	return (0);
}