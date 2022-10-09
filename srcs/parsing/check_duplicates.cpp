/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_duplicates.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 15:23:15 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/09 16:08:43 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Parsing.hpp"

bool	check_name(std::vector<Server>& servers)
{
	std::string	tmp;

	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		for (std::vector<Server>::iterator j = it + 1; j != servers.end(); j++)
		{
			if ((*j).getName() == (*it).getName())
				return (false);
		}
	}
	return (true);
}

bool	check_port(std::vector<Server>& servers)
{
	std::string	tmp;

	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); it++)
	{
		for (std::vector<Server>::iterator j = it + 1; j != servers.end(); j++)
		{
			if ((*j).getPort() == (*it).getPort())
				return (false);
		}
	}
	return (true);
}


bool	check_duplicates(std::vector<Server> servers)
{
	t_dup	tab[2] = {{"Multiple servers with same port in your configuration file, cannot procede.", &check_port},
					{"Multiple servers with same name in your configuration file, cannot procede.", &check_name}};

	if (servers.size() <= 1)
		return (true);
	for (int i = 0; i < 2; i++)
	{
		if (!tab[i].ft(servers))
		{
			std::cerr << RED << tab[i].error_msg << END << std::endl;
			return (false);
		}
	}
	return (true);
}