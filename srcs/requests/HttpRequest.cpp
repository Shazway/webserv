#include "HttpRequest.hpp"
#include <vector>
#include <algorithm>
#include <string>



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

HttpRequest::HttpRequest(Server &serv) : _serv(serv), _partiallyCompleted(false), _method(""), _path(""), _queryString(""), _httpVersion(""), _host(""), _connection(""), _contentType(""), _contentLength(0), _body("")
{
}


/*HttpRequest::HttpRequest( const HttpRequest & src )
{
}*/


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

HttpRequest::~HttpRequest()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

HttpRequest &				HttpRequest::operator=( HttpRequest const &rhs )
{
	if ( this != &rhs )
	{
		_partiallyCompleted = rhs._partiallyCompleted;
		_method = rhs._method;
		_path = rhs._queryString;
		_queryString = rhs._queryString;
		_httpVersion = rhs._httpVersion;
		_host = rhs._host;
		_connection = rhs._connection;
		_contentType = rhs._contentType;
		_contentLength = rhs._contentLength;
		_body = rhs._body;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, HttpRequest const & i )
{
	o << "PartiallyCompleted = " << (i.getPartiallyCompleted() ? "entame" : "pret ou vide") << std::endl;
	o << "Method = " << i.getMethod() << std::endl;
	o << "Path = " << i.getPath() << std::endl;
	o << "Query string = " << i.getQueryString() << std::endl;
	o << "HTTP version = " << i.getHttpVersion() << std::endl;
	o << "Host = " << i.getHost() << std::endl;
	o << "Connection = " << i.getConnection() << std::endl;
	o << "Content type = " << i.getContentType() << std::endl;
	o << "Content lengh = " << i.getContentLength() << std::endl;
	o << "Body = " << i.getBody() << std::endl;


	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


/*
** --------------------------------- ACCESSOR ---------------------------------
*/

bool		HttpRequest::getPartiallyCompleted() const
{
	return (_partiallyCompleted);
}

void		HttpRequest::setPartiallyCompleted(bool partiallyCmpleted)
{
	_partiallyCompleted = partiallyCmpleted;
}
std::string	HttpRequest::getMethod() const
{
	return (_method);
}

void		HttpRequest::setMethod(std::string method)
{
	if (!_serv.checkAllowedMethods(method, _path))
	{
		std::cout << method << std::endl;
		throw (ForbiddenMethodException());//ici, mettre l'erreur pour une methode interdite
	}
	_method = method;
}

std::string	HttpRequest::getPath() const
{
	return (_path);
}

void		HttpRequest::setPath(std::string path)
{
	size_t root = path.find("/");
	if (root == std::string::npos)
		throw (UnexpectedValueException()); //erreur de pas un chemin valide
	if (root == 0)
		_path = path;
	else
	{
		_host = path.substr(0, root - 1);
		_path = path.substr(root, std::string::npos);
	}
}

std::string	HttpRequest::getQueryString() const
{
	return (_queryString);
}

void		HttpRequest::setQueryString(std::string query)
{
	_queryString = query;
}

std::string	HttpRequest::getHttpVersion() const
{
	return (_httpVersion);
}

void		HttpRequest::setHttpVersion(std::string httpVersion)
{
	_httpVersion = httpVersion;
}

std::string	HttpRequest::getHost() const
{
	return (_host);
}

void		HttpRequest::setHost(std::string host)
{
	if (!_host.empty())
		throw (DoubleHostException()); //erreur double definition du host
	_host = host;
}

std::string	HttpRequest::getConnection() const
{
	return (_connection);
}

void		HttpRequest::setConnection(std::string connection)
{
	if (connection.compare("Keep-Alive"))
		throw (UnexpectedValueException());//erreur valeur inconnue
	_connection = connection;
}

std::string	HttpRequest::getContentType() const
{
	return (_contentType);
}

void		HttpRequest::setContentType(std::string contentType)
{
	_contentType = contentType;
}

unsigned int	HttpRequest::getContentLength() const
{
	return (_contentLength);
}

void		HttpRequest::setContentLength(std::string ContentLength)
{
	unsigned int	nb = atol(ContentLength.c_str());
	if (nb > _serv.getBody())
		throw (LongBodyException());//erreur de body trop long pour notre config
	_contentLength = nb;
}

std::string	HttpRequest::getBody() const
{
	return (_body);
}

void		HttpRequest::setBody(std::string body)
{
	_body += body;
}

/* ************************************************************************** */