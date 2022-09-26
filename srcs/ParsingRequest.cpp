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
    //  getline de request line
    //  getline de header
        
    }
    //recuperation du body
    //si on a recupere tout le body, mettre partiallyCompleted a 0, sinon 1

    //return le code d'erreur si souci, sinon 0


}