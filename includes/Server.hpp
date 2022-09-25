/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/24 17:32:21 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/25 17:09:02 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include <map>
#include "Colors.hpp"
#include <iterator>
#include "Routes.hpp"


class Server
{
private:
	std::string					root_path;
	std::string					name;
	std::string					ip;
	std::string					port;
	unsigned int				body_size;
	std::map<int, std::string>	error_paths;
	std::vector<Routes>			routes;
public:
	Server();
	~Server();
	unsigned int		getBody() const;
	std::string			getRootPath() const;
	std::string			getErrorPath(int code) const;
	std::string			getIp() const;
	std::string			getPort() const;
	std::string			getName() const;
	std::vector<Routes>	getRoutes() const;
	std::string			getErrorPath(int index) const;
	Routes const&		getRoute(int index);
	void			setBody(unsigned int body);
	void			setRootPath(std::string path);
	void			addErrorPath(int error, std::string path);
	void			setIp(std::string Ip);
	void			setPort(std::string port);
	void			setName(std::string name);
	void			addRoute(Routes const& Routes);
	void			checkIndex(int index);
};

class	WrongIndexForRootVectorException : public std::exception{
	virtual const char*	what() const throw();
};
const	char*	WrongIndexForRootVectorException::what() const throw(){
	return ("Wrong index to get route /!\\");
}
std::ostream&	operator<<(std::ostream& os, Server const& Server);
#endif