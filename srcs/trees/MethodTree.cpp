/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodTree.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:35:42 by mdelwaul          #+#    #+#             */
/*   Updated: 2022/11/15 17:05:23 by tmoragli         ###   ########.fr       */
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
	std::map<std::string, t_allowedMethods>::const_iterator pin/* = _subdirectories.begin()*/;
	//std::vector<std::string>	v_path;
	//std::vector<std::string>	v_test;
	//int							res;


	do {
		pin = _subdirectories.find(path);
		if (pin != _subdirectories.end())
			return (*pin);
		path = path.substr(0, path.find_last_of("/"));
	} while (path.find("/") != std::string::npos);
	return (*(_subdirectories.begin()));
/*
	ft_split(path, v_path, "/");
	for (std::map<std::string, t_allowedMethods>::const_iterator it = _subdirectories.begin(); it != _subdirectories.end(); it++)
	{
		ft_split((*it).first, v_test, "/");
		res = cmp_vector(v_test, v_path);
		if (res <= 0)
			pin = it;
		if (!res)
			break;
	}
	return (*pin);*/
}


std::map<std::string, t_allowedMethods> MethodTree::getTree() const
{
	return (this->_subdirectories);
}

/* ************************************************************************** */