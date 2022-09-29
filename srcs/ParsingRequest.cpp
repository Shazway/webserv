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
	request.setMethod(bufferString.substr(0, i - 1));
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
	request.setPath(bufferString.substr(i, j - 1));
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
		request.setQueryString(bufferString.substr(i, j - 1));
	}
	while (*it == ' ')
	{
		it++;
		j++;
	}
	bufferString = bufferString.substr(j, std::string::npos);
	if (bufferString.find("HTTP") != 0 || bufferString.find('\n') < 8)
		return (CODE_NO_HTTP_VERSION);
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
	request.setHttpVersion(bufferString);

	return (0);
}

int	parsingHeader(HttpRequest &request, std::string &bufferString)
{
	std::string	headers[4] = {"Host: ", "Connection: ", "Content-Type: ", "Content-Length: "};
	int	(HttpRequest::ft[4])(std::string) = {&HttpRequest::setHost, &HttpRequest::setConnection, &HttpRequest::setContentType, &HttpRequest::setContentType};
	int	pos;
	std::string	current;
	int error = 0;

	while (bufferString.find("\n"))
	{
		pos = bufferString.find(" ");
		if (pos == std::string::npos)
			continue ;
		current = bufferString.substr(0, pos + 1); 
		for (int i = 0; i < 4; i++)
		{
			if (current == headers[i])
			{
				error =*ft[i](bufferString.substr(pos + 1), bufferString.find("\n"));
				if (error)
					return (error);
			}
		}
		bufferString = bufferString.substr(bufferString.find("\n") + 1, std::string::npos);
	}
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
    }
    //recuperation du body
    //si on a recupere tout le body, mettre partiallyCompleted a 0, sinon 1

    //return le code d'erreur si souci, sinon 0
	if (request.getContentLengh())
	{
		request.setBody(bufferString);
		if (request.getBody().length() >= request.getContentLengh())
			request.setPartiallyCompleted(false);
	}
	return (0);
}