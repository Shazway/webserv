/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:45:03 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/27 17:41:47 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Routes.hpp"
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

Routes	create_route(std::string methods)
{
	Routes	route;

	if (methods.find("GET") != std::string::npos)
		route.setGET(true);
	if (methods.find("POST") != std::string::npos)
		route.setPOST(true);
	if (methods.find("DELETE") != std::string::npos)
		route.setDELETE(true);
	route.setName("root");
	route.setPath("/");
	return (route);
}

void	fill_info(Server& Serv, std::string info)
{
	if (info == "listen")
		Serv.setPort(info.substr(info.find_first_of("0123456789"), info.size()));
	if (info == "server_name")
		Serv.setName(info.substr(info.find_first_not_of(' '), info.size()));
	if (info == "body_size")
		Serv.setBody(info.substr(info.find_first_not_of(' '), info.size()));
	if (info == "root")
		Serv.setRootPath(info.substr(info.find_first_not_of(' '), info.size()));
	if (info == "method")
		Serv.addRoute(create_route(info.substr(info.find_first_not_of(' '), info.size())));
	/*if (info == "autoindex")
		Serv.setAutoIndex(info.substr(info.find_first_not_of(' '), info.size()));*/
}

void	fill_serv(Server& Serv, std::vector<std::string> content)
{
	std::string	serv_info[7] = {"listen", "server_name", "root", "method",
								"body_size", "autoindex"};

	for (std::vector<std::string>::iterator i = content.begin(); i != content.end(); i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if ((*i).find(serv_info[j]) != std::string::npos)
			{
				try
				{
					fill_info(Serv, serv_info[j]);
				}
				catch(const std::exception& e)
				{
					std::cerr << RED << e.what() << END << std::endl;
				}
			}
		}
	}
}

void	setup_config(Server& Serv)
{
	std::vector<std::string>	content;
	std::ifstream				file;
	std::string	location_info[3] = {"subpath", "location_method", "index"};

	file.open(Serv.getConfigPath(), std::ios_base::in);
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