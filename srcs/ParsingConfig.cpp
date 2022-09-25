/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:45:03 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/25 22:18:30 by tmoragli         ###   ########.fr       */
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

void	check_elems(std::vector<std::string>	content)
{
	int	nb_elems;

	nb_elems = 0;
	for (std::vector<std::string>::iterator i = content.begin(); i != content.end(); i++){
		if ((*i).find("listen") || (*i).find("root"))
			nb_elems++;
	}
	if (nb_elems < 2)
		throw(MissingElementsException());
	if (nb_elems > 2)
		throw(TooManyElementsException());
}

void	setup_config(Server& Serv)
{
	std::vector<std::string>	content;
	std::ifstream				file;
	std::string	serv_info[7] = {"listen", "server_name", "root", "method", 
								 "body_size", "autoindex", "location"};
	std::string	location_info[3] = {"subpath", "method", "index"};

	file.open(Serv.getConfigPath(), std::ios_base::in);
	if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof())
	{
		std::cerr << RED
		<< "Failed to open config file from path: "
		<< Serv.getConfigPath() << END << std::endl;
	}
	else
		fill_content(file, content);
	try{check_elems(content);}
	catch(const std::exception& e)
	{
		std::cerr << RED << e.what() << END << std::endl;
	}
	file.close();
}