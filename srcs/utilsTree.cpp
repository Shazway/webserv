#include "utilsTree.hpp"

int	cmp_vector(std::vector<std::string> test, std::vector<std::string> comp)
{
	std::vector<std::string>::iterator	it_test = test.begin();
	std::vector<std::string>::iterator	it_comp = comp.begin();
	while (it_test != test.end() && it_comp != comp.end() && *it_test == *it_comp)
	{
		it_test++;
		it_comp++;
	}
	if (it_test != test.end() && it_comp != comp.end())
		return (strcmp(*it_test, *it_comp));
	if (it_test == test.end() && it_comp == comp.end())
		return (0);
	if (it_test != test.end())
		return (1);
	return (-1);
}