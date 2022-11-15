/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 21:44:21 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/13 22:28:07 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "HttpRequest.hpp"
#include <stdlib.h>
#include <vector>
#include "Colors.hpp"

class CgiHandler
{
private:
	char		**_args;
	int			_fd[2];
	int			_pid;
	HttpRequest	_request;
public:
	CgiHandler(HttpRequest request);
	~CgiHandler();
private:
	void	get_handler(HttpRequest request);
	void	post_handler(HttpRequest request);
	std::string str_convert(std::string arg);
	void	separate_args(std::string str);
	void	str_arr_free();
};

#endif