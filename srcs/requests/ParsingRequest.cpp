/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParsingRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdelwaul <mdelwaul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 15:35:02 by mdelwaul          #+#    #+#             */
/*   Updated: 2022/11/10 00:18:39 by mdelwaul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <map>

#include "Colors.hpp"
#include "HttpRequest.hpp"

int	parsingRequestLine(HttpRequest &request, std::string bufferString)
{
	int	 i = 0, j = 0;

	if (bufferString.empty())
		return (1);
	std::string::iterator it = bufferString.begin();
	while (*it >= 'A' && *it <= 'Z')
	{
		it++;
		i++;
	}
	if (i < 2)
		return (CODE_NO_METHOD);
	request.setMethod(bufferString.substr(0, i));
	while (*it == ' ')
	{
		it++;
		i++;
	}
	j = i;
	while (*it != ' ' && *it != '?')
	{
		it++;
		j++;
	}
	if (j - i < 1)
		return (CODE_NO_PATH);
	request.setPath(bufferString.substr(i, j - i));
	if (*it == '?')
	{
		it++;
		i = j + 1;
		j = i;
		while (*it != ' ')
		{
			it++;
			j++;
		}
		if (j - i >= 1)
			request.setQueryString(bufferString.substr(i, j));
	}
	while (*it == ' ')
	{
		it++;
		j++;
	}
	bufferString = bufferString.substr(j, bufferString.find("\r\n") - j);
	if (bufferString == "HTTP 1.0" || bufferString == "HTTP/1.0")
		request.setHttpVersion("1.0");
	else if (bufferString == "HTTP 1.1" || bufferString == "HTTP/1.1")
		request.setHttpVersion("1.1");
	else
		std::cout << "None" << std::endl;
	//else
	//	return (CODE_NO_HTTP_VERSION);

	/*if (bufferString.find("HTTP") != 0 || bufferString.find('\n') < 8)
		return (CODE_NO_HTTP_VERSION);
	std::cout << bufferString << std::endl;
	j = 0;
	it = bufferString.begin();
	while (j < 4 || *it == ' ')
	{
		j++;
		it++;
	}
	bufferString = bufferString.substr(j, bufferString.find('\n'));
	if (bufferString.compare("1.0") && bufferString.compare("1.1"))
		return (CODE_UNSUPPORTED_HTTP_VERSION);
	request.setHttpVersion(bufferString);*/

	return (0);
}

int	parsingHeader(HttpRequest &request, std::string bufferString)
{
	size_t	pos;
	std::string	current;

	while (!bufferString.empty())
	{
		pos = bufferString.find(" ");
		if (pos == std::string::npos)
			continue ;
		current = bufferString.substr(0, pos + 1); 
		if (current == "Host: ")
			request.setHost(bufferString.substr(pos + 1, bufferString.find("\n") - (pos + 1)));
		if (current == "Connection: ")
			request.setConnection(bufferString.substr(pos + 1, bufferString.find("\n") - (pos + 1)));
		if (current == "Content-Type: ")
			request.setContentType(bufferString.substr(pos + 1, bufferString.find("\n") - (pos + 1)));
		if (current == "Content-Length: ")
			request.setContentLength(bufferString.substr(pos + 1, bufferString.find("\n") - (pos + 1)));
		pos = bufferString.find("\n");
		if (pos == std::string::npos)
			bufferString = "";
		else
			bufferString = bufferString.substr(pos + 1, std::string::npos);
	}
	return (0);
}
//appele par la partie avec les sockets, stocker les httpRequest dans un map

size_t parsingRequest(HttpRequest &request, std::string &bufferString)
{
	std::string requestLine;
	/*std::cout << BLUE << "Parsing request recieved: \n" << bufferString << END << std::endl;*/
	if (bufferString.empty())
		return (0);
	// getline de request line
	int error = parsingRequestLine(request, bufferString);
	if (error)
		return (error);
// getline de header
	std::cout << YELLOW << "[" << request.getPath() << "]" << END << std::endl;

	size_t	n = bufferString.find("\n");
	//std::cout << RED << "Buffer string before: [" << bufferString << "]" << END << std::endl;
	bufferString = bufferString.substr(n + 1, std::string::npos);
	n = bufferString.find("\n\n") < bufferString.find("\r\n\r\n") ? bufferString.find("\n\n") : bufferString.find("\r\n\r\n");
	error = parsingHeader(request, bufferString.substr(0, n));
	if (error)
		return (error);
	if (n == std::string::npos)
		bufferString = "";
	else
		bufferString = bufferString.substr(n + (bufferString.find("\n\n") < bufferString.find("\r\n\r\n") ? 2 : 4), std::string::npos);
	//recuperation du body
	//si on a recupere tout le body, mettre partiallyCompleted a 0, sinon 1

	//return le code d'erreur si souci, sinon 0
	//std::cout << GREEN << "Buffer string whole: [" << bufferString << "]" << END << std::endl;
	if (request.getContentLength())
		request.setBody(bufferString.substr(0, request.getContentLength()));
	bufferString = bufferString.substr(request.getContentLength());
	//std::cout << BLUE << "Buffer string sub: " << "["<< request.getBody() << "]" << END << std::endl;
	if (bufferString.empty())
		std::cout << "C'est vide" << std::endl;
	return (0);
}