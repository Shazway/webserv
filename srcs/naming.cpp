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
	std::string name_test = name;
	int test = open(name.c_str(), O_RDONLY);
	//std::cout << (test == -1 ? "echec de l ouverture" : "c est ouvert") << std::endl;
	if (test == -1)
		return (name);
	while (test > 0)
	{
		number++;
		close(test);
		name_test = name.substr(0, name.find_last_of(".")) + " (" + itoa(number) + ")" + name.substr(name.find_last_of("."), std::string::npos);
		test = open(name_test.c_str(), O_RDONLY);
	}
	close(test);
	return (name_test);
}