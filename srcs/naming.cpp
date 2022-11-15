#include "Server.hpp"
#include "HttpRequest.hpp"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <sstream>
#include "Parsing.hpp"
#include "utilsTree.hpp"
#include "Webserv.hpp"

//lui envoyer le nom parse, va faire la fusion avec le chemin
std::string	namingFiles(Server serv, std::string tmpName)
{
	std::string name = serv.getRootPath();
	name.append(serv.getUploadPath());
	name.append("/");
	name.append(tmpName);
	//std::string nameNb = name;
	int	number = 0;
	int test = open(name.c_str(), O_RDONLY);
	//std::cout << (test == -1 ? "echec de l ouverture" : "c est ouvert") << std::endl;
	if (test == -1)
	{
		close(test);
		return (name);
	}
	while (test > 0)
	{
		number++;
		test = open((name + " (" + itoa(number) + ")").c_str(), O_RDONLY);
	}
	return (name + " (" + itoa(number) + ")");
}