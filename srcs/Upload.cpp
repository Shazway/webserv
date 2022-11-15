/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 17:26:28 by tmoragli          #+#    #+#             */
/*   Updated: 2022/11/15 16:13:54 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Upload.hpp"

std::string	namingFiles(Server serv, std::string tmpName);


Upload::Upload(): _path(), _complete(0), _file(){
	return ;
}
Upload::Upload(Server &server): _path(), _complete(0), _file()
{
	_serv = server;
	return ;
}

Upload::Upload(Upload const& assign)
{
	if (assign._file.is_open())
		this->openFile(namingFiles(_serv, assign._path.substr(assign._path.find_last_not_of("/"), assign._path.size() - assign._path.find_last_not_of("/")).c_str()));
	_path = assign._path;
	_delim = assign._delim;
	_complete = assign._complete;
	_serv = assign._serv;
}

Upload::~Upload(){
	if (_file.is_open())
		_file.close();
	return ;
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
	if (name.empty())
		return ;
	_file.open(namingFiles(_serv, name).c_str(), std::ios::out | std::ios::binary | std::ios::ate | std::ios::app);
	if (!_file.is_open())
	{
		std::cout << "Crash ici -> " << namingFiles(_serv, name) << std::endl;
		throw(FailedToOpenFileException());
	}
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

Upload&	Upload::operator=(Upload const& assign)
{
	if (assign._file.is_open())
		this->openFile(namingFiles(_serv, assign._path.substr(assign._path.find_last_not_of("/"), assign._path.size() - assign._path.find_last_not_of("/")).c_str()));
	_path = assign._path;
	_delim = assign._delim;
	_complete = assign._complete;
	_serv = assign._serv;
	return (*this);
}