/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 17:26:28 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/06 22:40:55 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Upload.hpp"

Upload::Upload(): _new_file(), _file(), _file_path(), _complete(0){
	return ;
}

Upload::~Upload(){
	if (_file.is_open())
		_file.close();
	return ;
}

std::ofstream	Upload::getFile() const
{
	return (_file);
}

std::string		Upload::getPath() const
{
	return (_path);
}

std::string		Upload::getDelim() const
{
	return (_delim);
}

bool			Upload::getComplete() const
{
	return (_complete);
}


void	Upload::openFile(std::string name)
{
	_file.open(name.c_str(), std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	if (!_file.is_open())
		throw(FailedToOpenFileException());
}

void	Upload::setPath(std::string path)
{
	_path = path;
}

void	Upload::closeFile()
{
	if (_file.is_open())
		_file.close();
}

void	Upload::addContent(std::string content)
{
	_file << content;
}

void	Upload::setDelim(std::string delim)
{
	_delim = delim;
}

bool	Upload::checkCompletion(std::string body)
{
	std::string	line;

	if (body.empty())
	{
		_completed = 0;
		return (false);
	}
	if (body.find(_delim + "--") != std::string::npos)
		return (true);
	return (false);
}
