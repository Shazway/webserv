#include "MethodTree.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

MethodTree::MethodTree()
{
}

MethodTree::MethodTree( const MethodTree & src )
{
	(void)src;
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
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	(void)rhs;
	return *this;
}

std::ostream &			operator<<( std::ostream & o, MethodTree const & i )
{
	//o << "Value = " << i.getValue();
	(void)i;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool	MethodTree::isAllowed(std::string method, std::string path)
{
	std::map<std::string, t_allowedMethods>::iterator it;
	it = _subdirectories.lower_bound(path);
	if (!method.compare("GET"))
		return ((*it).second.get);
	if (!method.compare("POST"))
		return ((*it).second.post);
	if (!method.compare("DELETE"))
		return ((*it).second.del);
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


/* ************************************************************************** */