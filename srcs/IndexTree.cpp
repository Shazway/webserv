#include "IndexTree.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

IndexTree::IndexTree()
{
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

void	IndexTree::addExecption(std::string path, std::string index)
{
	_subdirectories[path] = index;
}

std::map<std::string, std::string>	IndexTree::getTree() const
{
	return (_subdirectories);
}

/* ************************************************************************** */