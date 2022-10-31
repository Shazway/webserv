#include "IndexTree.hpp"
#include "utilsTree.hpp"
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


std::pair<std::string, std::string>	IndexTree::getClosestDirectory(std::string path) const
{
	std::map<std::string, std::string>::const_iterator pin = _subdirectories.begin();
	std::vector<std::string>	v_path;
	std::vector<std::string>	v_test;
	int							res;


	ft_split(path, v_path, "/");
	std::cout << "Path for closest: " << path << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _subdirectories.begin(); it != _subdirectories.end(); it++)
	{
		ft_split((*it).first, v_test, "/");
		res = cmp_vector(v_test, v_path);
		if (res <= 0)
			pin = it;
		if (!res)
			break;
	}
	std::cout << "Pin string 1: " << (*pin).first << " Pin string 2: " << (*pin).second << std::endl;
	return (*pin);
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