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
	std::cout << BLINK_RED << request.getQueryString() << END << std::endl;
}

void	CgiHandler::post_handler(HttpRequest request)
{
	std::cout << BLINK_CYAN << request.getBody() << END << std::endl;
}

//login=rdyrdrydrdr&password=dzqzzdq
//inox=hello+world