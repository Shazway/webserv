/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 22:04:43 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/18 19:50:06 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "Sockets.hpp"
#include <map>

Webserv::Webserv(int size): nb_servers(size), nb_events(EVENT_SIZE), epollfd(epoll_create(1000))
{
	servers = new Server[size];
	events = new epoll_event[nb_events];
	return ;
}

Webserv::~Webserv(){
	if (epollfd != -1)
		close(epollfd);
	delete[] servers;
	delete[] events;
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

void	Webserv::increaseNbEvent()
{
	nb_events++;
}

void	Webserv::decreaseNbEvent()
{
	nb_events--;
}

void	Webserv::setEpollfd(int fd)
{
	epollfd = fd;
	return ;
}

Server	Webserv::getServer(int index) const
{
	if (index < 0 || index > nb_servers)
		throw(WrongIndexForServers());
	return (servers[index]);
}
epoll_event	Webserv::getEvent(int index) const
{
	if (index < 0 || index > 1024)
		throw(WrongIndexForEvents());
	return (events[index]);
}
epoll_event*	Webserv::getEvents() const
{
	return (events);
}

void	Webserv::setServer(int index, Server const& server)
{
	if (index < 0 || index > nb_servers)
		throw(WrongIndexForServers());
	servers[index] = server;
}

void	Webserv::setEvent(int index, epoll_event const& event)
{
	if (index < 0 || index > 1024)
		throw(WrongIndexForEvents());
	events[index] = event;
}

void	Webserv::printServers(std::ostream& os) const
{
	for (int i = 0; i < nb_servers; i++)
	{
		os << RED << "Listen fd: [" << servers[i].getSocket() << "]" << END << std::endl;
		os << BLUE << "Serv info: " << END << servers[i] << END << std::endl;
	}
}
void	Webserv::add_event(int fd, int flag)
{
	struct epoll_event event;

	event.data.fd = fd;
	event.events = flag;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	increaseNbEvent();
	events[fd] = event;
}
void	Webserv::printEvents(std::ostream& os) const
{
	for (size_t i = 0; i < nb_events; i++)
	{
		os << WHITE << "Fd: " << i << " ";
		os << "FLAG: " << events[i].events << std::endl;
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