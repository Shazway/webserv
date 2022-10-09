#include "HttpAnswer.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

HttpAnswer::HttpAnswer()
{
}

HttpAnswer::HttpAnswer( const HttpAnswer & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

HttpAnswer::~HttpAnswer()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

HttpAnswer &				HttpAnswer::operator=( HttpAnswer const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, HttpAnswer const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */