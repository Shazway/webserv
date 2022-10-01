/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 20:57:36 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/01 23:36:37 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

const	char*	WrongIndexForRootVectorException::what() const throw(){
	return ("Wrong index to get route /!\\");
}

Server::Server(): name(), ip(), port(), body_size(), error_paths(), routes(){
	return ;
}
Server::Server(Server const& serv)
{
	*this = serv;
	return ;
}

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
std::string Server::getConfigPath() const{
	return (this->config_path);
}
std::string Server::getIp() const{
	return (this->ip);
}
int Server::getPort() const{
	return (this->port);
}
std::string Server::getName() const{
	return (this->name);
}
std::map<int, std::string> Server::getErrorPaths() const{
	return (this->error_paths);
}

void	Server::setBody(std::string body){
	body_size = atoi(body.c_str());
}

void	Server::setRootPath(std::string path){
	root_path = path;
}
void	Server::setConfigPath(std::string path){
	config_path = path;
}
void	Server::setIp(std::string Ip){
	this->ip = Ip;
}

void	Server::setPort(std::string port){
	this->port = atoi(port.c_str());
}

void	Server::setName(std::string name){
	this->name = name;
}

bool	Server::getAutoIndex() const
{
	return (this->auto_index);
}
void	Server::setAutoIndex(std::string autoindex)
{
	if (autoindex.find("true") != std::string::npos)
		this->auto_index = true;
	if (autoindex.find("false") != std::string::npos)
		this->auto_index = false;
}

Server& Server::operator=(Server const& assign)
{
	name = assign.name;
	ip = assign.ip;
	port = assign.port;
	body_size = assign.body_size;
	error_paths = assign.error_paths;
	routes = assign.routes;
	config_path = assign.config_path;
	method = assign.method;
	auto_index = assign.auto_index;
	return (*this);
}

bool			Server::checkAllowedMethods(std::string method, std::string path)
{
	return (routes.isAllowed(method, path));
}

std::ostream&	operator<<(std::ostream& os, Server const& Server)
{
	std::map<int, std::string> errors;

	errors = Server.getErrorPaths();
	os << GREEN << "Server name: " << Server.getName() << std::endl
	<< BLUE << "Port: " << Server.getPort() << std::endl
	<< GREEN << "IP: " << Server.getIp() << std::endl
	<< RED << "Body size: " << Server.getBody() << std::endl
	<< MAGENTA << "Root path: " << Server.getRootPath() << END << std::endl
	<< Server.routes << std::endl;
	for (std::map<int, std::string>::iterator i = errors.begin(); i != errors.end(); i++)
	{
		os << YELLOW << (*i).first << " ";
		os << YELLOW << (*i).second << std::endl;
	}
	os << END;
	return (os);
}