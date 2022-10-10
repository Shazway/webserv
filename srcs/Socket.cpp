#include "Socket.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Socket::Socket()
{
}

Socket::Socket( const Socket & src )
{
	*this = src;
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
	if ( this != &rhs )
	{
		this->_fd = rhs._fd;
		this->serv = rhs.serv;
		this->_addr.sin_family = rhs._addr.sin_family;
		this->_addr.sin_port = rhs._addr.sin_port;
		this->_addr.sin_addr = rhs._addr.sin_addr;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Socket const & i )
{
	o << "fd = " << i.getFd() << std::endl;
	o << "serv = " << i.getServ() << std::endl;
	o << "addr = " << i.getAddr()->sin_addr.s_addr << "::" << i.getAddr()->sin_port << std::endl;
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/
int	Socket::getFd() const
{
	return (_fd);
}

void	Socket::setFd(int fd)
{
	_fd = fd;
}

void	Socket::setServ(Server& server)
{
	serv = &server;
}
Server*	Socket::getServ() const
{
	return (serv);
}

const struct sockaddr_in* Socket::getAddr() const
{
	return (&_addr);
}

/* ************************************************************************** */