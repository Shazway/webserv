/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 20:57:36 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/18 21:47:30 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

const	char*	WrongIndexForRootVectorException::what() const throw(){
	return ("Wrong index to get route /!\\");
}

Server::Server(): name(), ip(), port(), body_size(), error_paths(), routes(){
	socket = -1;
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

size_t Server::getBody() const{
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

int Server::getSocket() const{
	return (this->socket);
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
void	Server::setSocket(int socket){
	this->socket = socket;
}
void	Server::setAddr(){
	addr.sin_family = AF_INET;
	addr.sin_port = htons((unsigned int)port);
	addr.sin_addr.s_addr = htonl(0);
}
struct sockaddr_in	Server::getAddr() const{
	return(addr);
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
	config_path = assign.config_path;
	root_path = assign.root_path;
	name = assign.name;
	ip = assign.ip;
	method = assign.method;
	port = assign.port;
	body_size = assign.body_size;
	error_paths = assign.error_paths;
	auto_index = assign.auto_index;
	addr = assign.addr;
	socket = assign.socket;
	routes = assign.routes;
	html = assign.html;
	return (*this);
}

bool			Server::checkAllowedMethods(std::string method, std::string path)
{
	return (routes.isAllowed(method, path));
}

std::ostream&	operator<<(std::ostream& os, Server const& serv)
{
	std::map<int, std::string> errors;

	errors = serv.getErrorPaths();
	os << GREEN << "Server name: " << serv.getName() << std::endl
	<< BLUE << "Port: " << serv.getPort() << std::endl
	<< GREEN << "IP: " << serv.getIp() << std::endl
	<< RED << "Body size: " << serv.getBody() << std::endl
	<< MAGENTA << "Routes: " << serv.getRootPath() << std::endl
	<< serv.routes << std::endl
	<< CYAN << "Indexes: " << serv.html << END << std::endl
	<< "Listen fd: " << serv.getSocket() << std::endl;
	for (std::map<int, std::string>::iterator i = errors.begin(); i != errors.end(); i++)
	{
		os << YELLOW << (*i).first << " ";
		os << YELLOW << (*i).second << std::endl;
	}
	os << END;
	return (os);
}


int	Server::init_socket()
{
	int			fd_listen;

	fd_listen = ::socket(AF_INET, SOCK_STREAM, 0);
	if (fd_listen == -1)
		return (-1);
	bool	trash = false;
	setsockopt(fd_listen, SOL_SOCKET, SO_REUSEADDR, (char*)&trash, sizeof(&trash));
	this->setAddr();
	fcntl(fd_listen, O_NONBLOCK);
	if (bind(fd_listen, (sockaddr*)&addr, sizeof(addr)) != 0)
		return (-1);
	if (listen(fd_listen, 5) < 0)
		return (-1);
	return (fd_listen);
}
