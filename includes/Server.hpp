/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 17:32:21 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/24 21:30:03 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include "Colors.hpp"
#include <iterator>

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
	std::string				root_path;
	std::string				name;
	std::string				ip;
	std::string				port;
	unsigned int			body_size;
	std::string				error_path;
	std::vector<t_routes>	routes;
public:
	Server();
	~Server();
	unsigned int	getBody() const;
	std::string		getRootPath() const;
	std::string		getErrorPath() const;
	std::string		getIp() const;
	std::string		getPort() const;
	std::string		getName() const;
	std::vector<t_routes>	getRoutes() const;
};
std::ostream&	operator<<(std::ostream& os, Server const& Server);
#endif