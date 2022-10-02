/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:45:03 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/02 20:35:56 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Exceptions.hpp"
#include "Parsing.hpp"
#include <iostream>
#include <fstream>

void	display_v_str(std::vector<std::string> v_str)
{
	for (v_str_it i = v_str.begin(); i != v_str.end(); i++)
		std::cout << (*i) << std::endl;
}

std::string	find_string_tab(std::string name, std::string *tab, int size){
	for (int i = 0; i < size; i++){
		if (tab[i] == name)
			return (tab[i]);
	}
	return ("");
}

void	fill_content(std::ifstream& file, std::vector<std::string>& content)
{
	std::string	line;

	while (std::getline(file, line)){
		content.push_back(line);
	}
}

void	add_method(Server& Serv, std::string content, std::string& path)
{
	t_allowedMethods	allowed;

	if (content.find("GET"))
		allowed.get = true;
	if (content.find("POST"))
		allowed.post = true;
	if (content.find("DELETE"))
		allowed.del = true;
	Serv.routes.addExecption(path, allowed.get, allowed.post, allowed.del);
}

void	add_error_code(Server& Serv, std::string content)
{
	int	code;
	std::string	path;
	std::string	tmp;

	tmp = content.substr(content.find_first_of("0123456789"), content.size());
	code = atoi(tmp.c_str());
	path = content.substr(content.find_last_of(' '), content.find_last_not_of(';'));
	std::cout << RED << path << END << std::endl;
	Serv.addErrorPath(code, path);
}

void	display_servers(std::vector<Server> servers)
{
	for (std::vector<Server>::iterator i = servers.begin(); i != servers.end(); i++)
		std::cout << (*i) << std::endl;
}

bool	parse_server(Server& serv, v_str content, v_str_it it, t_parser parse[8])
{
	v_str	args;

	if (it != content.end() && (*it).find("server:") != std::string::npos)
		it++;
	while (it != content.end() && (*it).find("server:") == std::string::npos)
	{
		for (int i = 0; i < 8; i++)
		{
			if ((*it).find(parse[i].serv_info))
			{
				ft_split((*it).data(), args, " ");
				args.erase(args.begin());
				if (!parse[i].s(args, serv))
					return (false);
			}
		}
	}
	return (true);
}

bool	fill_servers(std::vector<Server>& servers, v_str content, t_parser parse[8])
{
	v_str	args;
	Server	tmp_serv;

	for (v_str_it it = content.begin(); it != content.end(); it++)
	{
		if (it == content.end())
			break ;
		if ((*it).find("server:"))
		{
			if (!parse_server(tmp_serv, content, it, parse))
				return (false);
			servers.push_back(tmp_serv);
			if (it != content.end())
				it++;
		}
	}
	return (true);
}

std::vector<Server>	setup_config(char* config_path)
{
	std::vector<Server>			servers;
	std::ifstream				file;
	v_str						content;
	t_parser	parse[8] = {{"error_path", &add_errorpath}, {"ip", &set_ip},
							{"name", &set_name}, {"root", set_root},
							{"method", &add_method}, {"body_size", &set_bodysize},
							{"autoindex", &set_autoIndex}, {"port", &set_port}};

	file.open(config_path, std::ios_base::in);
	if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof())
	{
		std::cerr << RED
		<< "Failed to open config file from path: "
		<< config_path << END << std::endl;
		return (servers);
	}
	fill_content(file, content);
	file.close();
	if (!fill_servers(servers, content, parse))
	{
		std::cerr << RED << "Error while parsing file, see error message above" << END << std::endl;
		return (servers);
	}
	display_servers(servers);
	return (servers);
}