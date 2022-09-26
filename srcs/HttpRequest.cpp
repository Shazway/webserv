#include "HttpRequest.hpp"
#include <vector>
#include <algorithm>
#include <string>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

HttpRequest::HttpRequest() : _partiallyCompleted(false)
{
}

HttpRequest::HttpRequest( const HttpRequest & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

HttpRequest::~HttpRequest()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

HttpRequest &				HttpRequest::operator=( HttpRequest const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, HttpRequest const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

bool		HttpRequest::getPartiallyCompleted()
{
	return (_partiallyCompleted);
}

void		HttpRequest::setPartiallyCompleted(bool partiallyCmpleted)
{
	_partiallyCompleted = partiallyCmpleted;
}
std::string	HttpRequest::getMethod()
{
	return (_method);
}

void		HttpRequest::setMethod(std::string method)
{
	if (!_serv.checkAllowedMethods(method, _path))
		throw ();//ici, mettre l'erreur pour une methode interdite
	_method = method;
}

std::string	HttpRequest::getPath()
{
	return (_path);
}

void		HttpRequest::setPath(std::string path)
{
	int root = path.find("/");
	if (root == std::string::npos)
		throw (); //erreur de pas un chemin valide
	if (root == 0)
		_path = path;
	else
	{
		_host = path.substr(0, root - 1);
		_path = path.substr(root, std::string::npos);
	}
}

std::string	HttpRequest::getQueryString()
{
	return (_queryString);
}

void		HttpRequest::setQueryString(std::string query)
{
	_queryString = query;
}

std::string	HttpRequest::getHttpVersion()
{
	return (_httpVersion);
}

void		HttpRequest::setHttpVersion(std::string httpVersion)
{
	if (httpVersion.compare("HTTP/1.1") && httpVersion.compare("HTTP/1.0"))
		throw ();//erreur de version HTTP invalide
	_httpVersion = httpVersion;
}

std::string	HttpRequest::getHost()
{
	return (_host);
}

void		HttpRequest::setHost(std::string host)
{
	if (!_host.empty())
		throw (); //erreur double definition du host
	_host = host;
}

std::string	HttpRequest::getConnection()
{
	return (_connection);
}

void		HttpRequest::setConnection(std::string connection)
{
	if (connection.compare("Keep-Alive"))
		throw ();//erreur valeur inconnue
	_connection = connection;
}

std::string	HttpRequest::getContentType()
{
	return (_contentType);
}

void		HttpRequest::setContentType(std::string contentType)
{
	_contentType = contentType;
}

unsigned int	HttpRequest::getContentLengh()
{
	return (_contentLengh);
}

void		HttpRequest::setContentLengh(std::string contentLengh)
{
	unsigned int	nb = std::stoul(contentLengh);
	if (nb > serv.getBody())
		throw ();//erreur de body trop long pour notre config
	_contentLengh = nb;
}

std::string	HttpRequest::getBody()
{
	return (_body);
}

void		HttpRequest::setBody(std::string body)
{
	_body = body;
}

/* ************************************************************************** */