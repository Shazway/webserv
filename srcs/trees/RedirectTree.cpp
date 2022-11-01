#include "RedirectTree.hpp"
#include <map>
#include <string>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

RedirectTree::RedirectTree()
{
}

RedirectTree::RedirectTree( const RedirectTree & src )
{
	(void)src;
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
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, RedirectTree const & i )
{
	(void)i;
	//o << "Value = " << i.getValue();
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

	tmp.first = path;
	tmp.second = destination;
	_errorcodes[error].insert(tmp);
}


/* ************************************************************************** */