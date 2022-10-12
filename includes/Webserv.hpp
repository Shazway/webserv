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
	size_t							nb_events;
	int								epollfd;
public:
	std::vector<struct epoll_event>		events;//garder en vector, ne pas mettre en map (pour epoll_wait)
	Webserv();
	~Webserv();
//getters
	void								add_event(int fd, int flag);
	void								add_server(int fd, Server serv);

	size_t								getNbEvents() const;
	int									getEpollfd() const;
//set
	void	setNbEvents(size_t nb);
	void	increaseNbEvent();
	void	decreaseNbEvent();
	void	setEpollfd(int fd);
//affichage
	void	printServers(std::ostream& os) const;
	void	printEvents(std::ostream& os) const;
};

std::ostream&	operator<<(std::ostream& os, Webserv const& webserv);

#endif