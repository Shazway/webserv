/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BookMark.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 17:12:43 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/09 22:33:54 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOOK_MARK
#define BOOK_MARK

#include "HttpRequest.hpp"
#include "Server.hpp"

class BookMark
{
private:
	int			ret;
	int			fd;
	HttpRequest request;
public:
	BookMark(Server& serv);
	BookMark(BookMark const& copy);
	~BookMark();
	int		getFd() const;
	void	setFd(int fd);
	int		getRet() const;
	void	setRet(int ret);
	HttpRequest	getRequest() const;
	void		setRequest(HttpRequest const& HttpRequest);
	BookMark& operator=(BookMark const& assign);
};

std::ostream&	operator<<(std::ostream& os, BookMark const& bookmark);

#endif