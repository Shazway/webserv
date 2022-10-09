#include "Socket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Socket::Socket()
{
}

Socket::Socket( const Socket & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Socket::~Socket()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Socket &				Socket::operator=( Socket const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Socket const & i )
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
int	Socket::getFd()
{
	return (_fd);
}

void	Socket::setFd(int fd)
{
	_fd = fd;
}

void	Socket::setServ(Server const& server)
{
	serv = &server;
}
Server&	Socket::getServ() const
{
	return (serv);
}

struct sockaddr_in& Socket::getAddr()
{
	return (&_addr);
}

/* ************************************************************************** */