/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:45:03 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/27 19:54:52 by tmoragli         ###   ########.fr       */
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

void	fill_info(Server& Serv, std::string info, std::string content)
{
	if (info == "port")
		Serv.setPort(content.substr(content.find_first_of("0123456789"), content.size()));
	if (info == "server_name")
		Serv.setName(content.substr(content.find_first_not_of(' '), content.size()));
	if (info == "body_size")
		Serv.setBody(content.substr(content.find_first_not_of(' '), content.size()));
	if (info == "root")
		Serv.setRootPath(content.substr(content.find_first_not_of(' '), content.size()));
	/*	(void)Serv;
		std::cout << " " << BLUE << info << END << std::endl;
		(void)content;*/
}

void	fill_serv(Server& Serv, std::vector<std::string> content)
{
	std::string	serv_info[7] = {"port", "server_name", "root", "method",
								"body_size", "autoindex", "host"};

	for (std::vector<std::string>::iterator i = content.begin(); i != content.end(); i++)
	{
		//std::cout << GREEN << (*i).data() << END << std::endl;
		for (int j = 0; j < 6; j++)
		{
			if ((*i).find(serv_info[j]) != std::string::npos)
			{
				//std::cout << "Here j is " << j << RED << (*i).data() << END << std::endl;
				fill_info(Serv, serv_info[j], (*i).substr((*i).find_first_of(' '), (*i).find_first_of(';') - 1));
			}
		}
	}
}

void	setup_config(Server& Serv)
{
	std::vector<std::string>	content;
	std::ifstream				file;
	std::string	location_info[3] = {"subpath", "location_method", "index"};

	std::cout << Serv.getConfigPath() << std::endl;
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