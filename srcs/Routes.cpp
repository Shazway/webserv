#include "Routes.hpp"
#include <iostream>
#include <string>

//CREER L'ARBRE TOUS LES DOSSIERS SONT DES NOEUDS//
Routes::Routes()
{
	std::string	tab[3] = {"GET", "POST", "DELETE"};
	t_method	tmp;

	for (int i = 0; i < 3; i++){
		tmp.name = tab[i];
		tmp.rule = false;
		methods.push_back(tmp);
	}
}

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

void	Routes::setAutoindex(bool autoindex)
{
	_auto_index = autoindex;
}

bool	Routes::getGET() const
{
	return (methods[GET].rule);
}

void	Routes::setGET(bool get)
{
	methods[GET].rule = get;
}

bool	Routes::getPOST() const
{
	return (methods[POST].rule);
}

void	Routes::setPOST(bool post)
{
	methods[POST].rule = post;
}

bool	Routes::getDELETE() const
{
	return (methods[DELETE].rule);
}

void	Routes::setDELETE(bool del)
{
	methods[DELETE].rule = del;
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
