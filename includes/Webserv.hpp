/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 21:43:06 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/16 23:17:14 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "Sockets.hpp"
#include "Server.hpp"
#include <map>
#define BUFFER_SIZE 2048
#define EVENT_SIZE 1024

typedef struct epoll_event epoll_event;

class WrongIndexForServers : virtual public std::exception
{
	virtual const char* what() const throw()
	{
		return ("Wrong index for servers");
	}
};
class WrongIndexForEvents : virtual public std::exception
{
	virtual const char* what() const throw()
	{
		return ("Wrong index for events");
	}
};

class Webserv
{
private:
	int						nb_servers;
	size_t					nb_events;
	int						epollfd;
	Server					*servers;
	epoll_event				*events;
public:
	Webserv(int size);
	~Webserv();
//getters
	Server								getServer(int index) const;
	epoll_event							getEvent(int index) const;
	epoll_event*						getEvents() const;
	size_t								getNbEvents() const;
	int									getEpollfd() const;

//set
	void		add_event(int fd, int flag);
	void		setNbEvents(size_t nb);
	void		setServer(int index, Server const& Server);
	void		setEvent(int index, epoll_event const& event);
	void		increaseNbEvent();
	void		decreaseNbEvent();
	void		setEpollfd(int fd);
//affichage
	void	printServers(std::ostream& os) const;
	void	printEvents(std::ostream& os) const;
};

std::ostream&	operator<<(std::ostream& os, Webserv const& webserv);

#endif