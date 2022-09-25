/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 20:57:36 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/24 21:45:50 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(): name(), ip(), port(), body_size(), error_path(), routes(){return ;}

Server::~Server(){
	//free mem ?//
	return ;
}

unsigned int Server::getBody() const{
	return (this->body_size);
}
std::string Server::getErrorPath() const{
	return (this->error_path);
}
std::string Server::getRootPath() const{
	return (this->root_path);
}
std::string Server::getIp() const{
	return (this->ip);
}
std::string Server::getPort() const{
	return (this->port);
}
std::string Server::getName() const{
	return (this->name);
}
std::vector<Routes> Server::getRoutes() const{
	return (this->routes);
}

std::ostream&	operator<<(std::ostream& os, Server const& Server)
{
	std::vector<Routes> routes;

	routes = Server.getRoutes();
	os << GREEN << "Server name: " << Server.getName() << std::endl
	<< RED << "Body size: " << Server.getBody() << std::endl
	<< BLUE << "Port: " << Server.getPort() << std::endl
	<< GREEN << "IP: " << Server.getIp() << END << std::endl;
	for (std::vector<Routes>::iterator i = routes.begin(); i != routes.end(); i++){
		os << YELLOW << *i << END << std::endl;
	}
	return (os);
}