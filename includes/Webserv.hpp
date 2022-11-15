/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 21:43:06 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/15 18:35:51 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <map>
# include "Server.hpp"
# include "HttpRequest.hpp"
# include "Upload.hpp"
# define BUFFER_SIZE 4096
# define EVENT_SIZE 1024

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
	int						max_event;
public:
	Webserv();
	~Webserv();
//getters
	std::map<int, HttpRequest>	requests;
	std::map<int, std::string>	answers;
	std::map<int, Upload>		uploads;
	Server								getServer(int index) const;
	epoll_event							getEvent(int index) const;
	epoll_event*						getEvents() const;
	size_t								getNbEvents() const;
	int									getEpollfd() const;
	int									getMaxEvent() const;

//set
	void		allocating(int size);
	void		add_event(int fd, int flag);
	void		remove_event(int fd);
	void		setNbEvents(size_t nb);
	void		setServer(int index, Server const& Server);
	void		setEvent(int index, epoll_event const& event);
	void		setEpollfd(int fd);
	void		setMaxEvent(int event);
//affichage
	void	printServers(std::ostream& os) const;
	void	printEvents(std::ostream& os) const;
};

std::ostream&	operator<<(std::ostream& os, Webserv const& webserv);

std::string		itoa(long nb);

#endif