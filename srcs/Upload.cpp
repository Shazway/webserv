/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 17:26:28 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/06 17:48:07 by tmoragli         ###   ########.fr       */
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

std::ifstream	Upload::getFile() const
{
	return (_file);
}

std::string		Upload::getPath() const
{
	return (_path);
}

bool			Upload::getComplete() const
{
	return (_complete);
}


void	Upload::createFile(std::string name)
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

void	Upload::checkCompletion()
{
	
}