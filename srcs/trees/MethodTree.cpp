/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodTree.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:35:42 by mdelwaul          #+#    #+#             */
/*   Updated: 2022/11/19 22:23:58 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Colors.hpp"
#include "MethodTree.hpp"
#include "Parsing.hpp"
#include "utilsTree.hpp"
#include <vector>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

MethodTree::MethodTree()
{
	t_allowedMethods methods;
	memset(&methods, 0, sizeof(t_allowedMethods));
	_subdirectories["/"] = methods;
}

MethodTree::MethodTree( const MethodTree & src )
{
	_subdirectories = src._subdirectories;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

MethodTree::~MethodTree()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

MethodTree &				MethodTree::operator=( MethodTree const & rhs )
{
	if ( this != &rhs )
		_subdirectories = rhs._subdirectories;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, MethodTree const & i )
{
	std::map<std::string, t_allowedMethods> tree = i.getTree();

	for (std::map<std::string, t_allowedMethods>::iterator it = tree.begin(); it != tree.end(); it++)
	{
		o << "Path: " << (*it).first << std::endl;
		o << "GET: " << ((*it).second.get ? "enabled" : "disabled") << ", ";
		o << "POST: " << ((*it).second.post ? "enabled" : "disabled") << ", ";
		o << "DEL: " << ((*it).second.del ? "enabled" : "disabled") << std::endl;
	}
	o << END;
	return (o);
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool	MethodTree::isAllowed(std::string method, std::string path)
{
	std::pair<std::string, t_allowedMethods> it;
	it = this->getClosestDirectory(path);
	if (!method.compare("GET"))
		return (it.second.get);
	if (!method.compare("POST"))
		return (it.second.post);
	if (!method.compare("DELETE"))
		return (it.second.del);
	return (false);
}

void	MethodTree::addExecption(std::string path, bool get, bool post, bool del)
{
	t_allowedMethods allow;
	allow.get = get;
	allow.post = post;
	allow.del = del;
	_subdirectories[path] = allow;
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::pair<std::string, t_allowedMethods>	MethodTree::getClosestDirectory(std::string path) const
{
	std::map<std::string, t_allowedMethods>::const_iterator pin;

	do {
		pin = _subdirectories.find(path);
		if (pin != _subdirectories.end())
			return (*pin);
		path = path.substr(0, path.find_last_of("/"));
	} while (path.find("/") != std::string::npos);
	return (*(_subdirectories.begin()));
}


std::map<std::string, t_allowedMethods> MethodTree::getTree() const
{
	return (this->_subdirectories);
}

/* ************************************************************************** */