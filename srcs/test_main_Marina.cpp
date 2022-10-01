#include "HttpRequest.hpp"
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "Colors.hpp"

int parsingRequest(HttpRequest &request, std::string buffer);

int	main(int ac, char** av)
{
	Server			serv;
	HttpRequest		request(serv);
	char * buffer = new char [2048];
	std::ifstream	file(buffer, std::ifstream::binary);

	if (ac == 1)
		std::ifstream file("test_socket.txt" , std::ifstream::binary);
	else
		std::ifstream file(av[1] , std::ifstream::binary);
	if (!std::istream::read(buffer, 2048))
	{
		std::cout << RED
		<< "Failed to read the test file " << END << std::endl;
		delete[] buffer;
		return (0);
	}
	if (parsingRequest(request, buffer))
	{
		std::cout << RED
		<< "Failed to parse " << END  << std::endl << request << std::endl;
		delete[] buffer;
		return (0);
	}
	std::cout << RED
		<< "Victoire (en theorie) " << END  << std::endl << request << std::endl;
	delete[] buffer;
	return (0);
}