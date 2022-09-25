/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmoragli <tmoragli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 21:26:18 by tmoragli          #+#    #+#             */
/*   Updated: 2022/09/25 22:16:50 by tmoragli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP
#include <exception>

class	TooManyElementsException : public std::exception{
	virtual const char*	what() const throw();
};

class	MissingElementsException : public std::exception{
	virtual const char*	what() const throw();
};

const	char*	TooManyElementsException::what() const throw(){
	return ("/!\\ Duplicate elements in config file.. Can't procede");
}
const	char*	MissingElementsException::what() const throw(){
	return ("/!\\ Missing elements in config file.. Can't procede\nExamples: listen, root path");
}

#endif