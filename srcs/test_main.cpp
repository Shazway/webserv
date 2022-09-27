/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 17:33:01 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/27 18:39:20 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Parsing.hpp"

int	main(int ac, char **av)
{
	Server	server;

	if (ac == 2)
	{
		std::string path = av[1];
		std::cout << MAGENTA << path << " " << av[1] << END << std::endl;
		server.setConfigPath(path);
	}
	setup_config(server);
	return (0);
}