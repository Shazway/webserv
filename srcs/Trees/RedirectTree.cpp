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
	return *this;
}

std::ostream &			operator<<( std::ostream & o, RedirectTree const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

std::string	RedirectTree::redirectTo(int error, std::string path)
{
	std::string	above;
	std::map<int, std::string>::iterator it;

	it = _errorcodes.find(error);
	if (it != _errorcodes.end())
	{
		//chercher avec le path actuel
		if ((*it).second.find(path) != (*it).second.end())
			return (*it).second[path];
		//chercher un dossier plus haut
		if (path.findlastof("/") != 0)
		{
			above = path.substr(0, path.findlastof("/"));
			above = redirectTo(error, above);
		}
	}
	return (deeper);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void	RedirectTree::addRedirect(int error, std::string path, std::string destination)
{
	std::map<std::string, std::string> tmp;

	tmp[path] = destination;
	_errorcodes[error].insert(tmp);
}


/* ************************************************************************** */