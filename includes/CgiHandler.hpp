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
	int			_stdin;
	int			_pid;
public:
	CgiHandler(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answer);
	~CgiHandler();
	void	get_handler(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answer);
	void	post_handler(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answer);
private:
	void	separate_args(std::string str, HttpRequest request);
	void	exec_cgi(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answer);
	void	exec_child();
	void	str_arr_free();
	std::string str_convert(std::string arg);
	void	generate_success(int fd, std::map<int, std::string>& answers, std::string str);
};

void	gen_error(std::map<int, HttpRequest>::iterator &it, std::map<int, std::string>& answers, int code);

#endif