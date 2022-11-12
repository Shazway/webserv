/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post_parse.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdelwaul <mdelwaul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 18:36:06 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/10 12:01:20 by mdelwaul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>

#include "Parsing.hpp"
#include "Server.hpp"
#include "Upload.hpp"

void	get_filename(std::string line, std::string& filename)
{
	v_str	split_info;

	ft_split(line, split_info, ";");
	for (v_str_it it = split_info.begin(); it != split_info.end(); it++)
	{
		if ((*it).find("filename=") != std::string::npos)
		{
			std::cout << "Avant 1" << std::endl;
			filename = (*it).substr((*it).find_first_of('"') + 1, (*it).find_last_of('"') - ((*it).find_first_of('"') + 1));
			std::cout << "Apres 1" << std::endl;
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
		if (pos != std::string::npos)
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
		if (pos != 0)
			pos--;
	}
	return (pos);
}

int	add_incomplete_content(Upload& up, std::string content)
{
	//std::cout << YELLOW << "Incomplete, <" << content << ">" << std::endl;
	up.addContent(content);
	return (INCOMPLETE);
}

int	add_complete_content(Upload& up, std::string content)
{
	size_t	line;

	//std::cout << MAGENTA << "str before substr :" << content << "." << std::endl;
	line = find_last_line(content, 3);
	std::cout << "Avant 2" << std::endl;
	up.addContent(content.substr(0, line));
	std::cout << "Apres 2" << std::endl;
	//std::cout << CYAN << "substr :" << content.substr(0, line) << "." << std::endl;
	up.closeFile();
	return (COMPLETE);
}

void	init_upload(Upload& up, v_str lines, std::string content, std::string& data, size_t &line)
{
	std::string	filename;

	line = find_first_line(content, 1);
	std::cout << "Avant 3" << std::endl;
	get_filename(content.substr(line, find_first_line(content, 2) - line), filename);
	std::cout << "Apres 3" << std::endl;
	up.setPath(filename); // Setup du filepath (il faut changer le path brut par ce qu'il y a dans serv)
	std::cout << "Avant 4" << std::endl;
	up.setDelim(lines.front().substr(0, lines.front().find("\r")));// Setup du delimiteur
	std::cout << "Apres 4" << std::endl;
	data = content.substr(find_first_line(content, 4), content.size() - content.find(up.getDelim()));
	up.openFile(up.getPath());
}

int	upload(Upload& up, std::string const& content) //J'aurai du l'appeler download T_T
{
	std::string	data;
	v_str		lines;
	size_t		line;

	//std::cout << RED << "on a un fichier" << std::endl;
	ft_split(content, lines, "\n");
	if (content.empty())
	{
		std::cout << "mechant upload" << std::endl;
		return (INCOMPLETE);
	}
	if (up._file.is_open()) // Si on a déjà un fichier, alors il est incomplet
	{
		//std::cout << RED << "en fait c etait open" << std::endl;
		if (content.find(up.getDelim() + "--") == std::string::npos)// Il s'apprete a être complet ?
			return (add_incomplete_content(up, content)); //non -> on ajoute au fichier et on return incomplet
		else
			return (add_complete_content(up, content));//oui -> on ajoute au fichier et on return complet
	}
	else // Nouveau upload tout frais
	{
		init_upload(up, lines, content, data, line); // On choppe, nom de fichier, path, contenu du début, delimiteur
	//std::cout << BLUE << "Entre dans upload <" << data << ">" << up.getDelim() <<"?" << std::endl << data.find(up.getDelim()) << std::endl;
		if (data.find(up.getDelim() + "--") != std::string::npos) // Il y a déjà le delimiteur ?
			return (add_complete_content(up, data)); //oui-> On ajoute le contenu entier et on return complet
		else
			return (add_incomplete_content(up, data));//non-> On ajoute le contenu et on return incomplet
	}
}
