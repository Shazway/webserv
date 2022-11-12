/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:33:01 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/12 21:23:22 by tmoragli         ###   ########.fr       */
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

void	separate_lines(std::vector<std::string> &lines, std::string content)
{
	size_t	n;
	while (!content.empty())
	{
		n = find_first_line(content, 1);
		lines.push_back(content.substr(0, n));
		//std::cout << find_first_line(content, 1) << std::endl;
		if (n != std::string::npos)
			content = content.substr(n, std::string::npos);
		else
			content.clear();
		//std::cout << "while:" << content << "." << std::endl;
	}
	//std::cout << BLUE << "Sorti du while" << std::endl;
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
	//std::cout << "Start[" << RED << str << END << "]end" << std::endl;
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
	std::cout << "Start[" << RED << "PASS" << END << "]end" << std::endl;
		tmp = (*it).substr(16, i);
	std::cout << "Start[" << RED << "PASSED" << END << "]end" << std::endl;
		bodySize=atoi(tmp.c_str());

		if (bodySize > maxBodySize) //si bodySize est plus grand que la limite, true pour code d'erreur apres
			return (true);
	}
	while (it != v.end() && !is_empty((*it)))
		it++;
	if (bodySize == 0)
		return (true);
	it++;
	//std::cout << str << std::endl;
	while (it != v.end() && bodySize > (*it).size())
	{
		bodySize -= ((*it).size());
		it++;
	}
	//std::cout << "Body size: " << bodySize << std::endl;
	//std::cout << "return : " << (it == v.end() ? "false" : "true") << std::endl;
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
			answers.erase(it);
		}
	}
}

void	generate_ok(int fd, std::map<int, std::string>& answers, std::ifstream& file, std::string type)
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
	if (type == "favicon")
		answers[fd] += "Content-Type: html/favicon.ico\n";
	else if (type == "html")
		answers[fd] += "Content-Type: text/html\n";
	answers[fd] += "Content-Length: ";
	answers[fd] += itoa((long)content.size());
	answers[fd] += "\n\n";
	answers[fd] += content;
}

void	gen_body_too_long(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answers)
{
	std::cout << WHITE << "trop long"<< END << std::endl;
	answers[(*it).first] = "HTTP/1.1 413 Request Entity Too Large\n\n";
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
				generate_ok((*it).first, answers, file, "html");
		}
		else if (file.is_open())
		{
			if ((*it).second.getPath().find("favicon") != std::string::npos)
				generate_ok((*it).first, answers, file, "favicon");
			else
				generate_ok((*it).first, answers, file, "html");
		}
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
		if ((*it).second.getContentType().find("multipart/form-data") != std::string::npos)// Faut download
		{
			Upload	up; //Crée une instance de upload pour l'ajouter si il existe pas dans map

		//std::cout << RED << "here" << END << std::endl;
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
			answers[(*it).first] += "Content-Length: 0\n\n";
			//
			// METTRE UN VRAI BODY
			//
		}
	}
}

void	answers_gen(std::map<int, HttpRequest>& requests, std::map<int, std::string>& answers, std::map<int, Upload>& uploads, std::map<int, Server> &client_serv)
{
	//gerer ici si le body est trop grand
	for (std::map<int, HttpRequest>::iterator it = requests.begin(); it != requests.end(); it++)
	{
		if ((*it).second.getBody().size() > client_serv[(*it).first].getBody())
			gen_body_too_long(it, answers);
		else if ((*it).second.getMethod() == "GET")
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
					read_char = 1;
					while (!complete_request(buffer_strings[client], client_serv[client].getBody()))
					{
						//std::cout << "Requete incomplete ou <" << buffer_strings[client] << "> est vide" << std::endl;
						memset(buff, 0, BUFFER_SIZE + 1);
						//memset(buff, 0, sizeof(char) * (BUFFER_SIZE + 1));
						//std::cout << std::endl << "Read char: " << read_char<< std::endl << std::endl;
						read_char = recv(client, buff, BUFFER_SIZE, MSG_DONTWAIT);//errors to check
						std::cout << std::endl << "Read char: " << read_char<< std::endl << std::endl;
						//if return 0 close la connection
						if (read_char <= 0)
						{
							std::cout << "Read a return: " << read_char << std::endl;
							closeFd(client);
							requests.erase(client);
						}
						if (read_char <= 0)
							break;
						buffer_strings[client].append(buff, read_char);
					}
					found = true;
					//std::cout << WHITE <<" Hey there [" << buffer_strings[client] << "]" << END << std::endl;
				}
			}
		}
		//stocker un fd max, pour opti et pas passer sur les 1000 fd
		for (int i = 0; i <= fdMax ; i++)
		{
			/*if (!buffer_strings[client].empty())
				std::cout << GREEN << complete_request(buffer_strings[client]) << buffer_strings[client] << END << std::endl;*/
			//pour une raison inconnue, complete_request renvoie false au lieu de true sur les POST
			if (complete_request(buffer_strings[i], client_serv[i].getBody()))
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
		answers_gen(requests, answers, uploads, client_serv);
		send_answers(answers);
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