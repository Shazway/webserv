/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 17:32:21 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/25 21:32:00 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include <map>
#include "Colors.hpp"
#include <iterator>
#include "MethodTree.hpp"
#include <string.h>
#include <MethodTree.hpp>
#include <stdlib.h>
#include "Parsing.hpp"
#include <fstream>
#include <sys/epoll.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "IndexTree.hpp"
#include "RedirectTree.hpp"

//Vector string//
typedef std::vector<std::string> v_str;
//Vector string iterator//
typedef std::vector<std::string>::iterator v_str_it;


class Server
{
private:
	std::string					config_path;
	std::string					root_path;
	std::string					name;
	std::string					ip;
	std::string					method;
	int							port;
	size_t				body_size;
	std::map<int, std::string>	error_paths;
	bool						auto_index;
	struct sockaddr_in			addr;
	int							socket;
public:
	Server();
	Server(Server const& serv);
	~Server();
	int					init_socket();
	size_t				getBody() const;
	struct sockaddr_in	getAddr() const;
	bool				getAutoIndex() const;
	std::string			getRootPath() const;
	std::string			getConfigPath() const;
	std::string			getIp() const;
	int					getPort() const;
	std::string			getName() const;
	int					getSocket() const;
	std::string			getErrorPath(int code) const;
	std::map<int, std::string>	getErrorPaths() const;
	void	setSocket(int socket);
	void	setAddr();
	void	setBody(std::string body);
	void	setRootPath(std::string path);
	void	setConfigPath(std::string path);
	void	addErrorPath(int error, std::string path);
	void	setIp(std::string Ip);
	void	setPort(std::string port);
	void	setName(std::string name);
	void	checkIndex(int index);
	void	setAutoIndex(std::string autoindex);
	bool	checkAllowedMethods(std::string method, std::string path);
	MethodTree		routes;
	IndexTree		html;
	RedirectTree	redirect;
	Server&	operator=(Server const& assign);
};

class	WrongIndexForRootVectorException : public std::exception{
	virtual const char*	what() const throw();
};

std::ostream&	operator<<(std::ostream& os, Server const& Server);
#endif