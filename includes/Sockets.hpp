/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Sockets.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/11 19:20:00 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/11 21:42:52 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKETS_HPP
# define SOCKETS_HPP

#include <fstream>
#include <sys/epoll.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "Colors.hpp"
#include "Server.hpp"

# define NB_EVENTS 1000
# define MAX_SIZE 1024
# define SIZE_FD 1000
# define BUFFER 1024 * 1024

class Sockets
{
private:
	int					fd_listen;
	int					fd_epoll;
	struct epoll_event	events[MAX_SIZE];
	Server				serv;

private:
	void	add_event(int fd, int flag);

public:
	Sockets();
	~Sockets();

	bool	init_server();
	void	start_server();
};

#endif