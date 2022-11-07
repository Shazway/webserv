/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_parse.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:36:06 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/07 23:38:57 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utilsTree.hpp"



void	get_filename(std::string line, std::string& filename)
{
	v_str	split_info;

	ft_split(line, split_info, ";");
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

int	add_incomplete_content(upload& up, std::string content)
{
	up.addContent(content);
	return (INCOMPLETE);
}

int	add_complete_content(Upload& up, std::string content)
{
	std::string	line;

	line = find_last_line(content, 3);
	up.addContent(content.substr(0, line));
	up.closeFile();
	return (COMPLETE)
}

void	init_upload(Upload& up, v_str lines, std::string content, std::string& data, size_t &line)
{
	std::string	filename;

	line = find_first_line(content, 1);
	filename = get_filename(content.substr(line, find_first_line(content, 2) - line), filename);
	up.setPath("uploaded_content/" + filename); // Setup du filepath (il faut changer le path brut par ce qu'il y a dans serv)
	up.setDelim(lines.front())// Setup du delimiteur
	data = content.substr(find_first_line(content, 4), content.size() - content.find(up.getDelim()));
	up.openFile(up.getPath());
}

int	upload(Upload& up, std::string const& content)
{
	std::string	data;
	v_str		lines;
	size_t		line;

	ft_split(content, lines, "\n");
	if (up.getFile().is_opened())
	{
		if (content.find(up.getDelim() + "--") == std::string::npos)
			return (add_incomplete_content(content));
		else
			return (add_complete_content(up, content));
	}
	else
	{
		init_upload(up, lines, content, data, line);
		if (content.find(up.getDelim() + "--"))
			return (add_complete_content(up, data));
		else
			return (add_incomplete_content(up, data.substr(0, line)));
	}
}
