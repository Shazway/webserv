#include "MethodTree.hpp"
#include "Colors.hpp"

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
	{
		_subdirectories = rhs._subdirectories;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, MethodTree const & i )
{
	std::map<std::string, t_allowedMethods> tree = i.getTree();

	for (std::map<std::string, t_allowedMethods>::iterator it = tree.begin(); it != tree.end(); it++)
	{
		o << GREEN << "Path: " << (*it).first << std::endl;
		o << "GET: " << ((*it).second.get ? "enabled" : "disabled") << ", ";
		o << "POST: " << ((*it).second.post ? "enabled" : "disabled") << ", ";
		o << "DEL: " << ((*it).second.del ? "enabled" : "disabled") << END << std::endl;
	}
	return (o);
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
std::map<std::string, t_allowedMethods> MethodTree::getTree() const
{
	return (this->_subdirectories);
}

/* ************************************************************************** */