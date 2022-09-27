#include "HttpRequest.hpp"
#include "Server.hpp"
#include <iostream>
#include <string>
#include <map>

//appele par la partie avec les sockets, stocker les httpRequest dans un map


int parsingRequest(HttpRequest &request, char* buffer)
{
    std::string bufferString = buffer;
    std::string requestLine;
    if (bufferString.empty())
        return (0);
    //normalement le buffer est assez grand pour avoir tout le header
    //si !partiallyCompleted
    if (!request.getPartiallyCompleted())
    {
		request.setPartiallyCompleted(true);
    //  getline de request line
        int     i = 0, j = 0;
        std::string::iterator it = bufferString.begin();
        while (*it >= "A" && *it <= "Z")
		{
			it++;
			i++;
		}
		if (i < 2)
			return (CODE_NO_METHOD);
		request.setMethod(bufferString.substr(0, i - 1));
		while (*it == " ")
		{
			it++;
			i++;
		}
		j = i;
		while (*it != " " && *it != "?")
		{
			it++;
			j++;
		}
		if (j - i < 1)
			return (CODE_NO_PATH);
		request.setPath(bufferString.substr(i, j - 1));
		if (*it == "?")
		{
			it++;
			i = j + 1;
			j = i;
			while (*it != " ")
			{
				it++;
				j++;
			}
			if (j - i < 1)
				return (CODE_NO_QUERY);
			request.setQueryString(bufferString.substr(i, j - 1));
		}
		while (*it == " ")
		{
			it++;
			j++;
		}
		bufferString = bufferString.substr(j, std::string::npos);
		if (bufferString.find("HTTP") != 0 || bufferString.find('\n') < 8)
			return (CODE_NO_HTTP_VERSION);
		j = 0;
		it = bufferString.begin();
		while (j < 4 || *it == " ")
		{
			j++;
			it++;
		}
		bufferString = bufferString.substr(j, bufferString.find('\n'));
		if (bufferString.compare("1.0") && bufferString.compare("1.1"))
			return (CODE_UNSUPPORTED_HTTP_VERSION);
		request.setHttpVersion(bufferString);

    //  getline de header
        
    }
    //recuperation du body
    //si on a recupere tout le body, mettre partiallyCompleted a 0, sinon 1

    //return le code d'erreur si souci, sinon 0


}