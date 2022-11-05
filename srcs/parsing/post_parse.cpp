/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_parse.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:36:06 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/05 15:07:19 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utilsTree.hpp"

void	get_filename(std::string line, std::string& filename)
{
	v_str	split_info;

	ft_split(line, split_info, ";");
	std::cout << BLUE << line << END << std::endl;
	for (v_str_it it = split_info.begin(); it != split_info.end(); it++)
	{
		if ((*it).find("filename=") != std::string::npos)
		{
			filename = (*it).substr((*it).find_first_of('"') + 1, (*it).find_last_of('"') - ((*it).find_first_of('"') + 1));
			break ;
		}
	}
}

size_t	find_first_line(std::string const& content, size_t line)
{
	size_t pos = 0;

	for (size_t i = 0; i < line; i++)
	{
		pos = content.find("\n", pos);
		pos++;
	}
	return (pos);
}

size_t	find_last_line(std::string const& content, size_t line)
{
	size_t pos = 0;

	for (size_t i = line; i > 0; i--)
	{
		pos = content.find_last_of("\n", pos);
		pos--;
	}
	return (pos);
}

void	upload(std::string const& content)
{
	std::ofstream	new_file;
	std::string	filename;
	std::string	delim;
	std::string	data;
	v_str		lines;
	size_t		line;

	ft_split(content, lines, "\n");
	if (lines.size() <= 5)
		return ;
	std::cout << RED << "start[" << "Data first: " << content << "]end" << END << std::endl;
	line = find_first_line(content, 1);
	get_filename(content.substr(line, find_first_line(content, 2) - line), filename);
	delim = lines.front();
	data = content.substr(find_first_line(content, 4), content.size() - content.find(delim));
	new_file.open(filename.c_str(), std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	new_file << data.substr(0, line);
	new_file.close();
}