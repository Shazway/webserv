/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 19:23:32 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/11 21:31:53 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Sockets.hpp"

Sockets::Sockets()
{
	return ;
}

Sockets::~Sockets()
{
	if (fd_listen > 0)
	{
		close(fd_listen);
		fd_listen = -1;
	}
	return ;
}


bool	Sockets::init_server()
{
	fd_listen = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_listen == -1)
	{
		std::cerr << RED << "Failed to create socket" << END << std::endl;
		return (false);
	}

	bool	trash = false;
	setsockopt(fd_listen, SOL_SOCKET, SO_REUSEADDR, (char*)&trash, sizeof(&trash));

	sockaddr_in	addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons((unsigned int)serv.getPort());
	addr.sin_addr.s_addr = htonl(0);

	if (bind(fd_listen, (sockaddr*)&addr, sizeof(addr)) != 0)
	{
		std::cerr << RED
		<< "Binding with port failed for port: [" << serv.getPort() << "]"
		<< END << std::endl;
		return (false);
	}

	if (listen(fd_listen, 5) < 0)
	{
		std::cerr << RED
		<< "Listen failed"
		<< END << std::endl;
		return (false);
	}

	fd_epoll = epoll_create(SIZE_FD);
	add_event(fd_listen, EPOLLIN);
	return (true);
}

void	Sockets::start_server()
{
	while (true)
	{
		int ret = epoll_wait(fd_epoll, events, NB_EVENTS, -1);
	
		for (int i = 0; i < ret; i++)
		{
			if (events[i].data.fd == fd_epoll)
			{
				sockaddr_in	addr;
				socklen_t	len = sizeof(addr);
				int client = accept(fd_listen, (sockaddr*)&addr, &len);
				if (client <= 0)
				{
					std::cerr << RED << "/!\\ Accept for client failed /!\\" << END << std::endl;
					continue ;
				}
				// Client accepted here
				add_event(client, EPOLLIN);
			}
		}
	}
}

//PRIVATE MANAGEMENT FUNCTIONS FOR EVENTS
void	Sockets::add_event(int fd, int flag)
{
	struct epoll_event event;

	event.data.fd = fd;
	event.events = flag;
	epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd, &event);
}