/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 22:04:43 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/12 00:07:56 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Sockets.hpp"
#include <map>

Webserv::Webserv(): servers(), events(), nb_events(0), epollfd(epoll_create(1000))
{
	return ;
}

Webserv::~Webserv(){
	if (epollfd != -1)
		close(epollfd);
	return ;
}


size_t	Webserv::getNbEvents() const
{
	return (nb_events);
}
int	Webserv::getEpollfd() const
{
	return (epollfd);
}

void	Webserv::setNbEvents(size_t nb)
{
	nb_events = nb;
	return ;
}

void	Webserv::setEpollfd(int fd)
{
	epollfd = fd;
	return ;
}

void	Webserv::add_event(int fd, int flag)
{
	struct epoll_event event;

	event.data.fd = fd;
	event.events = flag;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	events.insert(std::pair<int, struct epoll_event> (fd, event));
}

void	Webserv::add_server(int fd, Server serv)
{
	servers.insert(std::pair<int, Server> (fd, serv));
}

void	Webserv::printServers(std::ostream& os) const
{
	for (std::map<int, Server>::const_iterator it = servers.begin(); it != servers.end(); it++)
	{
		os << RED << "Listen fd: [" << (*it).first << "]" << END << std::endl;
		os << BLUE << "Serv info: " << END << (*it).second << END << std::endl;
	}
}

void	Webserv::printEvents(std::ostream& os) const
{
	for (std::map<int, struct epoll_event>::const_iterator it = events.begin(); it != events.end(); it++)
	{
		os << WHITE << "Fd: " << (*it).second.data.fd<< std::endl;
		os << "FLAG: " <<(*it).second.events << std::endl;
	}
}

std::ostream&	operator<<(std::ostream& os, Webserv const& webserv)
{
	os << CYAN << "Servers info: " << END << std::endl;
	webserv.printServers(os);
	webserv.printEvents(os);
	os << "Number of sockets: [" << webserv.getNbEvents() << "]" << std::endl;
	os << "Epollfd: [" << webserv.getEpollfd() << "]" << std::endl;
	os << END;
	return (os);
}