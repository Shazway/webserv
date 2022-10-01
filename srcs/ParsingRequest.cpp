#include "HttpRequest.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>
#include <map>


int	parsingRequestLine(HttpRequest &request, std::string bufferString)
{
	int     i = 0, j = 0;
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
		if (j - i < 1)
			return (CODE_NO_QUERY);
		request.setQueryString(bufferString.substr(i, j));
	}
	while (*it == ' ')
	{
		it++;
		j++;
	}
	std::cout << "coucou" << std::endl;
	bufferString = bufferString.substr(j, bufferString.find("\n") - j);
	std::cout << "coucou2" << std::endl;
	if (bufferString == "HTTP 1.0" || bufferString == "HTTP/1.0")
		request.setHttpVersion("1.0");
	else if (bufferString == "HTTP 1.1" || bufferString == "HTTP/1.1")
		request.setHttpVersion("1.1");
	std::cout << "coucou3" << std::endl;

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

int	parsingHeader(HttpRequest &request, std::string &bufferString)
{
	int	pos;
	std::string	current;
	int error = 0;

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

int parsingRequest(HttpRequest &request, std::string bufferString)
{
    std::string requestLine;
    if (bufferString.empty())
        return (0);
    //normalement le buffer est assez grand pour avoir tout le header
    //si !partiallyCompleted
    if (!request.getPartiallyCompleted())
    {
    //  getline de request line
		request.setPartiallyCompleted(true);
        int error = parsingRequestLine(request, bufferString);
		if (error)
			return (error);
    //  getline de header

		int	n = bufferString.find("\n");
		bufferString = bufferString.substr(n + 1, std::string::npos);
		error = parsingHeader(request, bufferString);
		if (error)
			return (error);
	std::cout << "coucou4" << std::endl;
    }
    //recuperation du body
    //si on a recupere tout le body, mettre partiallyCompleted a 0, sinon 1

    //return le code d'erreur si souci, sinon 0
	if (request.getContentLength())
	{
		request.setBody(bufferString);
		if (request.getBody().length() >= request.getContentLength())
			request.setPartiallyCompleted(false);
	}
	return (0);
}