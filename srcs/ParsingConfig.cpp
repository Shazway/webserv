/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingConfig.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 17:45:03 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/25 18:03:33 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Routes.hpp"
#include "Server.hpp"

std::string	find_string_tab(std::string name, std::string *tab, int size){
	for (int i = 0; i < size; i++){
		if (tab[i] == name)
			return (tab[i]);
	}
	return ("");
}

void	setup_config(Server& Serv)
{
	std::string	line;
	std::string	serv_info[7] = {"listen", "server_name", "root", "method", 
								 "body_size", "autoindex", "location"};
	std::string	location_info[3] = {"subpath", "method", "index"};
	bool		set[7];

	memset(set, 0, sizeof(bool) * 7);
	
}