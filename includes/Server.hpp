/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 17:32:21 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/08 16:49:59 by tmoragli         ###   ########.fr       */
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
#include "IndexTree.hpp"

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
	unsigned int				body_size;
	std::map<int, std::string>	error_paths;
	bool						auto_index;
public:
	Server();
	Server(Server const& serv);
	~Server();
	unsigned int		getBody() const;
	bool				getAutoIndex() const;
	std::string			getRootPath() const;
	std::string			getConfigPath() const;
	std::string			getIp() const;
	int					getPort() const;
	std::string			getName() const;
	std::string			getErrorPath(int code) const;
	std::map<int, std::string>	getErrorPaths() const;
	void			setBody(std::string body);
	void			setRootPath(std::string path);
	void			setConfigPath(std::string path);
	void			addErrorPath(int error, std::string path);
	void			setIp(std::string Ip);
	void			setPort(std::string port);
	void			setName(std::string name);
	void			checkIndex(int index);
	void			setAutoIndex(std::string autoindex);
	bool			checkAllowedMethods(std::string method, std::string path);
	MethodTree		routes;
	IndexTree		html;
	Server&	operator=(Server const& assign);
};

class	WrongIndexForRootVectorException : public std::exception{
	virtual const char*	what() const throw();
};

std::ostream&	operator<<(std::ostream& os, Server const& Server);
#endif