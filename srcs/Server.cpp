/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 20:57:36 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/25 19:28:52 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(): name(), ip(), port(), body_size(), error_paths(), routes(){return ;}

Server::~Server(){
	return ;
}

unsigned int Server::getBody() const{
	return (this->body_size);
}
std::string Server::getErrorPath(int code) const{
	std::map<int, std::string>::const_iterator i = error_paths.find(code);
	if (i == error_paths.end())
		return ("");
	return ((*i).second);
}
void Server::addErrorPath(int error, std::string path){
	try{error_paths[error] = path;}
	catch(const std::exception& e){
		std::cerr << RED << e.what() << END << std::endl;
	}
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
void	Server::checkIndex(int index){
	if (index < 0 || index > this->routes.size())
		throw(WrongIndexForRootVectorException());
}
Routes const&	Server::getRoute(int index){
	try{checkIndex(index);}
	catch(const std::exception& e){
		std::cerr << RED << e.what() << END << std::endl;
	}
	return (this->routes[index]);
}

void	Server::setBody(unsigned int body){
	body_size = body;
}

void	Server::setRootPath(std::string path){
	root_path = path;
}

void	Server::setPort(std::string port){
	this->port = port;
}
void	Server::setName(std::string name){
	this->name = name;
}
void	Server::addRoute(Routes const& Routes){
	this->routes.push_back(Routes);
}

std::string	getRouteName(std::string path)
{
	if (path.empty())
		return ("");
	return (path.substr(path.rfind('/'), path.size() - path.rfind('/')));
}

bool		checkMethodRule(Routes const route, std::string method)
{
	if (method == "GET")
		return (route.getGET());
	else if (method == "POST")
		return (route.getPOST());
	else if (method == "DELETE")
		return (route.getDELETE());
}

bool	Server::checkAllowedMethods(std::string method, std::string path)
{
	//ADD ERROR CODE IF NOT A REAL METHOD//
	if (method != "GET" && method != "POST" && method != "DELETE")
		return (false);
	std::string	folder_name = getRouteName(path);
	std::vector<Routes>::reverse_iterator	i = routes.rbegin();

	while (i != routes.rend()){
		if ((*i).getName() == folder_name)
			break;
		i++;
	}
	if (i == routes.rend())
		return (checkMethodRule((*i), method));
	while (i != routes.rend()){
		if (check)
	}
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