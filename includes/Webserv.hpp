/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 21:43:06 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/12 00:07:11 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "Sockets.hpp"
#include "Server.hpp"
#include <map>
class Webserv
{
private:
	std::map<int, Server>			servers;
	std::map<int, struct epoll_event>	events;
	size_t							nb_events;
	int								epollfd;
public:
	Webserv();
	~Webserv();
//getters
	void								add_event(int fd, int flag);
	void								add_server(int fd, Server serv);
	
	size_t								getNbEvents() const;
	int									getEpollfd() const;
//set
	void	setNbEvents(size_t nb);
	void	setEpollfd(int fd);
};

std::ostream&	operator<<(std::ostream& os, Webserv const& webserv);

#endif