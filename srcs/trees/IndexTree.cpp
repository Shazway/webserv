/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndexTree.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:35:32 by mdelwaul          #+#    #+#             */
/*   Updated: 2022/11/15 17:51:57 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>

#include "IndexTree.hpp"
#include "Parsing.hpp"
#include "utilsTree.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

IndexTree::IndexTree()
{
	_subdirectories["/"] = "";
}

IndexTree::IndexTree( const IndexTree & src )
{
	_subdirectories = src._subdirectories;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

IndexTree::~IndexTree()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

IndexTree &				IndexTree::operator=( IndexTree const & rhs )
{
	if ( this != &rhs )
	{
		_subdirectories = rhs._subdirectories;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, IndexTree const & i )
{
	std::map<std::string, std::string> tree = i.getTree();

	for (std::map<std::string, std::string>::iterator it = tree.begin(); it != tree.end(); it++)
	{
		o << (*it).first << " subdirectory has an index in " << (*it).second << std::endl;
	}
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


std::pair<std::string, std::string>	IndexTree::getClosestDirectory(std::string path) const
{
	/*std::map<std::string, std::string>::const_iterator pin = _subdirectories.begin();
	std::vector<std::string>	v_path;
	std::vector<std::string>	v_test;
	int							res;


	ft_split(path, v_path, "/");
	for (std::map<std::string, std::string>::const_iterator it = _subdirectories.begin(); it != _subdirectories.end(); it++)
	{
		ft_split((*it).first, v_test, "/");
		res = cmp_vector(v_test, v_path);
		if (res <= 0)
			pin = it;
		if (!res)
			break;
	}
	return (*pin);*/
	std::map<std::string, std::string>::const_iterator pin;
	
	do {
		pin = _subdirectories.find(path);
		if (pin != _subdirectories.end())
			return (*pin);
		path = path.substr(0, path.find_last_of("/"));
	} while (path.find("/") != std::string::npos);
	return (*(_subdirectories.begin()));
}

void	IndexTree::addExecption(std::string path, std::string index)
{
	_subdirectories[path] = index;
}

std::map<std::string, std::string>	IndexTree::getTree() const
{
	return (_subdirectories);
}

/* ************************************************************************** */