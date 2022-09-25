#include "MethodTree.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

MethodTree::MethodTree()
{
}

MethodTree::MethodTree( const MethodTree & src )
{
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
	return *this;
}

std::ostream &			operator<<( std::ostream & o, MethodTree const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

bool	MethodTree::isAllowed(std::string method, std::string path)
{

}

void	MethodTree::addExecption(std::string path, bool get, bool post, bool del)
{
	t_allowedMethods	allowed;
	allowed.GET = get;
	_subdirectories[path] = allowed;
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */