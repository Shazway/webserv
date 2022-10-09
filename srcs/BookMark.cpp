/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BookMark.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/09 17:21:04 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/09 17:34:40 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BookMark.hpp"

BookMark::BookMark(): fd(0), ret(0), request(){
	return ;
}

BookMark::~BookMark(){
	return ;
}

BookMark::BookMark(BookMark const& copy){
		copy = *this;
	return ;
}

void	BookMark::setFd(int fd){
	this->fd = fd;
}

void	BookMark::setRet(int ret){
	this->ret = ret;
}

void	BookMark::setRequest(int request){
	this->request = request;
}

int	BookMark::getFd() const{
	return (fd);
}

int	BookMark::getRet() const{
	return (ret);
}

HttpRequest& BookMark::getRequest() const{
	return (request);
}

BookMark& BookMark::operator=(BookMark const& assign)
{
	if (this != &assign)
	{
		ret = assign.getRet();
		fd = assign.getFd();
		request = assign.getRequest();
	}
	return (*this);
}

std::ostream& operator<<(std::ostream& os, BookMark const& bookmark)
{
	os << WHITE << bookmark.getFd() << std::endl;
	os << WHITE << bookmark.getRet() << std::endl;
	os << WHITE << bookmark.getRequest() << std::endl;
	return (os);
}