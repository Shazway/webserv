/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 21:52:23 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/13 22:40:07 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CgiHandler.hpp"

char	**str_arr_free(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

int	atoi_hexa(std::string hex)
{
	int	res;

	res = 0;
	for (size_t i = 0; i < hex.size(); i++)
	{
		res *= 16;
		if (isalpha(hex[i]))
			res += hex[i] - 'A' + 10;
		else
			res += hex[i] - '0';
	}
	return res;
}

int	count_args(std::string str)
{
	int i = 1;
	long unsigned int pos = 0;
	const char *str_char = str.c_str();
	while (pos < str.size())
	{
		if (str_char[pos] == '&')
			i++;
		pos++;
	}
	return i;
}

CgiHandler::CgiHandler(HttpRequest request): _args(NULL), _request(request)
{
	if (request.getMethod() == "GET")
		get_handler(request);
	else if (request.getMethod() == "POST")
		post_handler(request);
	return;
}

CgiHandler::~CgiHandler()
{
	str_arr_free(_args);
	return ;
}

void	CgiHandler::get_handler(HttpRequest request)
{
	std::cout << RED << request.getQueryString() << END << std::endl;
	std::cout << "NB_ARGS = " << count_args(request.getQueryString()) << std::endl;
	std::cout << RED << clean_args(request.getQueryString()) << END << std::endl;
}

void	CgiHandler::post_handler(HttpRequest request)
{
	std::cout << CYAN << request.getBody() << END << std::endl;
	std::cout << "NB_ARGS = " << count_args(request.getBody()) << std::endl;
	std::cout << CYAN << clean_args(request.getBody()) << END << std::endl;
}

std::string CgiHandler::clean_args(std::string arg)
{
	std::string	new_arg;
	for (size_t i = 0; i < arg.size(); i++)
	{
		if (arg[i] != '+' && arg[i] != '%')
			new_arg += arg[i];
		else if (arg[i] == '+')
			new_arg += ' ';
		else if (arg[i] == '%')
		{
			int hexval = atoi_hexa(arg.substr(i + 1, 2));
			unsigned char c = (unsigned char)hexval;
			new_arg += c;
			i += 2;
		}
	}
	return new_arg;
}