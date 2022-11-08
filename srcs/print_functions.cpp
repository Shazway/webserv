/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_functions.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdelwaul <mdelwaul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/08 16:21:31 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/08 15:52:24 by mdelwaul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "Colors.hpp"
#include "Server.hpp"


void	print_separator()
{
	std::cout << WHITE << "~~~~~~~~~~~~~~~~~~" << END << std::endl;
}

void	display_v_str(std::vector<std::string> str)
{
	for (std::vector<std::string>::iterator i = str.begin(); i != str.end(); i++)
		std::cout << RED << "[" << (*i) << "]" << END << std::endl;
}

void	display_servers(std::vector<Server> servers)
{
	int count = 1;
	for (std::vector<Server>::iterator i = servers.begin(); i != servers.end(); i++)
	{
		print_separator();
		std::cout << BLUE << "Server number " << count << ":" << END << std::endl;
		std::cout << (*i) << std::endl;
		print_separator();
		count++;
	}
}