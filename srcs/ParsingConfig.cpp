/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:45:03 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/27 22:01:57 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"
#include "Exceptions.hpp"
#include <iostream>
#include <fstream>

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
	path = content.substr(content.find_last_of(' '), content.find_last_not_of(";"));
	std::cout << RED << path << END << std::endl;
	Serv.addErrorPath(code, path);
}

void	fill_info(Server& Serv, std::string info, std::string content, std::string& path)
{
	if (info == "port")
		Serv.setPort(content.substr(content.find_first_of("0123456789"), content.find_last_not_of(';')));
	if (info == "server_name")
		Serv.setName(content.substr(content.find_first_not_of(' '), content.find_last_not_of(';')));
	if (info == "body_size")
		Serv.setBody(content.substr(content.find_first_not_of(' '), content.find_last_not_of(';')));
	if (info == "root")
		Serv.setRootPath(content.substr(content.find_first_not_of(' '), content.find_last_not_of(';')));
	if (info == "host")
		Serv.setIp(content.substr(content.find_first_not_of(' '), content.find_last_not_of(';')));
	if (info == "error_path")
		add_error_code(Serv, content);
	if (info == "method")
		add_method(Serv, content, path);
}

void	fill_serv(Server& Serv, std::vector<std::string> content)
{
	std::string	serv_info[9] = {"port", "server_name", "root", "method",
								"body_size", "autoindex", "host", "error_path"};

	std::string	path = "/";
	for (std::vector<std::string>::iterator i = content.begin(); i != content.end(); i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if ((*i).find(serv_info[j]) != std::string::npos)
			{
				fill_info(Serv, serv_info[j], (*i).substr((*i).find_first_of(' '), (*i).find_first_of(';') - 1), path);
			}
		}
	}
}

void	setup_config(Server& Serv)
{
	std::vector<std::string>	content;
	std::ifstream				file;
	std::string	location_info[3] = {"subpath", "location_method", "index"};

	file.open(Serv.getConfigPath().c_str() , std::ios_base::in);
	if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof())
	{
		std::cerr << RED
		<< "Failed to open config file from path: "
		<< Serv.getConfigPath() << END << std::endl;
	}
	else
		fill_content(file, content);
	file.close();
	fill_serv(Serv, content);
	std::cout << Serv << std::endl;
}