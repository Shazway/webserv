/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_parse.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:36:06 by tmoragli          #+#    #+#             */
/*   Updated: 2022/10/31 19:22:27 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utilsTree.hpp"

void	get_filename(std::string line, std::string& filename)
{
	v_str	split_info;

	ft_split(line, split_info, ";");

	for (v_str_it it = split_info.begin(); it != spit_info.end(); it++)
	{
		if ((*it).find("filename=") != std::string::npos)
		{
			filename = (*it).substr((*it).find_last_of("="), (*it).size() - (*it).find_last_of("="));
			break ;
		}
	}
}

size_t	find_first_line(std::string const& content)
{
	size_t pos = 0;

	for (int i = 0; i < 4; i++)
		pos = content.find("\n", pos);
	return (pos);
}

void	upload(std::string const& content)
{
	std::string	filename;
	std::string	delim;
	std::string	data;
	v_str		lines;
	int			count = 0;

	ft_split(content, lines, "\n");
	if (lines.size() <= 5)
		return ;
	delim = lines.front();
	data = content.substr(find_first_line(content), content.find(delim));
}