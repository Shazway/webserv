#include "HttpRequest.hpp"
#include <string>
#include <map>
#include <iostream>
#include "Colors.hpp"

int parsingRequest(HttpRequest &request, char* buffer);

int	main(int ac, char** av)
{
	std::istream	file;
	HttpRequest		request;
	std::string		buffer;

	if (ac == 1)
		file.open("test_socket.txt" , std::ios_base::in);
	else
		file.open(av[1] , std::ios_base::in);
	if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof())
	{
		std::cout << RED
		<< "Failed to open the test file " << END << std::endl;
		return (0);
	}
	if (parsingRequest(request, std::istream::read(file, buffer)))
	{
		std::cout << RED
		<< "Failed to parse " << END  << std::endl << request << std::endl;
		return (0);
	}
	std::cout << RED
		<< "Victoire (en theorie) " << END  << std::endl << request << std::endl;
	return (0);
}