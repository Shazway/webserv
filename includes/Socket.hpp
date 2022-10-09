#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <iostream>
# include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class Socket
{

	public:

		Socket();
		Socket( Socket const & src );
		~Socket();

		Socket &		operator=( Socket const & rhs );
		int	getFd();
		Server& getServ() const;
		void	setServ(Server const& serv);
		void	setFd(int s);
		struct sockaddr_in& getAddr();

	private:
		int	_fd;
		Server* serv;
		struct sockaddr_in	_addr; //stocker les httpRequest dans un map

};

std::ostream &			operator<<( std::ostream & o, Socket const & i );

#endif /* ********************************************************** SOCKET_H */