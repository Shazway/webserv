/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdelwaul <mdelwaul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 21:52:43 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/08 15:55:06 by mdelwaul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <string>
#include "Server.hpp"

void	ft_split(std::string str, v_str& v, std::string delim)
{
	size_t i = 0;
	std::string	tmp;

	while ((i = str.find(delim)) != std::string::npos)
	{
		tmp = str.substr(0, i);
		v.push_back(tmp);
		str.erase(0, i + delim.size());
	}
	v.push_back(str);
}
