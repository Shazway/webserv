/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:33:01 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/09 20:33:10 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Parsing.hpp"

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
		std::vector<Server> servers(parse_config(av[1]));
	return (0);
}