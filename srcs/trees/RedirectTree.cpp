#include "RedirectTree.hpp"
#include <map>
#include <string>
#include "utilsTree.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

RedirectTree::RedirectTree()
{
}

RedirectTree::RedirectTree( const RedirectTree & src )
{
	this->_errorcodes = src._errorcodes;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

RedirectTree::~RedirectTree()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

RedirectTree &				RedirectTree::operator=( RedirectTree const & rhs )
{
	if ( this != &rhs )
		this->_errorcodes = rhs._errorcodes;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, RedirectTree const & i )
{
	std::map<int, std::map<std::string, std::string> > tree;

	tree = i.getTree();
	if (tree.empty())
	{
		std::cout << "Empty redirections" << std::endl;
		return (o);
	}
	for (std::map< int, std::map<std::string, std::string> >::iterator it = tree.begin(); it != tree.end(); it++)
	{
		o << "Error " << (*it).first << " :" << std::endl;
		for (std::map<std::string, std::string>::iterator ite = (*it).second.begin(); ite != (*it).second.end(); ite++)
		{
			o << "   " << (*ite).first << " redirects to " << (*ite).second << std::endl;
		}
	}
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

std::string	RedirectTree::redirectTo(int error, std::string path)
{
	std::string	above;
	std::map<int, std::map<std::string, std::string> >::iterator it;

	it = _errorcodes.find(error);
	if (it != _errorcodes.end())
	{
		//chercher avec le path actuel
		if ((*it).second.find(path) != (*it).second.end())
			return (*it).second[path];
		//chercher un dossier plus haut
		if (path.find_last_of("/") != 0)
		{
			above = path.substr(0, path.find_last_of("/"));
			above = redirectTo(error, above);
		}
	}
	return (above);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void	RedirectTree::addRedirect(int error, std::string path, std::string destination)
{
	std::pair<std::string, std::string> tmp;
	std::pair<int, std::map<std::string, std::string> > tmp2;

	tmp.first = path;
	tmp.second = destination;
	tmp2.first = error;
	tmp2.second.insert(tmp);
	_errorcodes.insert(tmp2);
}

std::map< int, std::map<std::string, std::string> >	RedirectTree::getTree(void) const
{
	return (_errorcodes);
}


/* ************************************************************************** */