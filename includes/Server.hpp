/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 17:32:21 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/24 20:47:33 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>

typedef struct s_routes
{
	std::string	name;
	std::string	path;
	std::string	default_file;
	bool		auto_index;
	bool		GET;
	bool		POST;
	bool		DELETE;
}	t_routes;

class Server
{
private:
	std::string		name = NULL;
	std::string		ip;
	std::string		port;
	unsigned int	body_size;
	std::string		error_path;
	std::vector<t_routes>	routes;
};

#endif