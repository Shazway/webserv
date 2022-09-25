#include "Routes.hpp"
#include <iostream>
#include <string>




std::string Routes::getName() const
{
	return (_name);
}

void	Routes::setName(std::string name)
{
	_name = name;
}

std::string Routes::getPath() const
{
	return (_path);
}

void	Routes::setPath(std::string path)
{
	_path = path;
}

std::string Routes::getDefaultFile() const
{
	return (_default_file);
}

void	Routes::setDefaultFile(std::string default_file)
{
	_default_file = default_file;
}

bool	Routes::getAutoindex() const
{
	return (_auto_index);
}

void	Routes::setAutoindex(bool index)
{
	_auto_index = index;
}

bool	Routes::getGET() const
{
	return (_GET);
}

void	Routes::setGET(bool get)
{
	_GET = get;
}

bool	Routes::getPOST() const
{
	return (_POST);
}

void	Routes::setPOST(bool post)
{
	_POST = post;
}

bool	Routes::getDELETE() const
{
	return (_DELETE);
}

void	Routes::setDELETE(bool del)
{
	_DELETE = del;
}



std::ostream&	operator<<(std::ostream& os, Routes const &route)
{
	os << "Name : " << route.getName() << std::endl;
	os << "Path: " << route.getPath() << std::endl;
	os << "Default file : " << route.getDefaultFile() << std::endl;
	os << "Autoindex : " << (route.getAutoindex() ? "yes" : "no") << std::endl;
	os << "GET : " << (route.getGET() ? "allowed" : "forbidden") << std::endl;
	os << "GET : " << (route.getPOST() ? "allowed" : "forbidden") << std::endl;
	os << "GET : " << (route.getDELETE() ? "allowed" : "forbidden") << std::endl;

	return (os);
}
