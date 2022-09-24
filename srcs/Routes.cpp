#include "Server.hpp"

std::ostream&	operator<<(std::ostream& os, Routes const &route)
{
    os << "Name : " << routes.getName() << std::endl;
    os << "Path: " << routes.getPath() << std::endl;
    os << "Default file : " << routes.getDefaultFile() << std::endl;
    os << "Autoindex : " << routes.getAutoindex() ? "yes" : "no" << std::endl;
    os << "GET : " << routes.getGET() ? "allowed" : "forbidden"<< std::endl;
    os << "GET : " << routes.getPOST() ? "allowed" : "forbidden"<< std::endl;
    os << "GET : " << routes.getDELETE() ? "allowed" : "forbidden"<< std::endl;

    return (os);
}
