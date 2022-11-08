/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilsTree.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdelwaul <mdelwaul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:36:06 by mdelwaul          #+#    #+#             */
/*   Updated: 2022/11/08 15:50:56 by mdelwaul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <string>
#include <vector>

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
		return (std::strcmp((*it_test).c_str(), (*it_comp).c_str()));
	if (it_test == test.end() && it_comp == comp.end())
		return (0);
	if (it_test != test.end())
		return (1);
	return (-1);
}