#include "HttpRequest.hpp"
#include <vector>
#include <algorithm>

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

HttpRequest::HttpRequest()
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
std::string	HttpRequest::getMethod()
{
	return (_method);
}

void		HttpRequest::setMethod(std::string method)
{
	std::vector<std::string> allowedMethods = getAllowedMethods(_path); //doit verifier les routes
	std::vector<std::string>::iterator it = find(allowedMethods.begin(), allowedMethods.end(), method);
	if (it == allowedMethods.end())
		throw ();//ici, mettre l'erreur pour une methode interdite
	_method = method;
}
std::string	HttpRequest::getPath();
void		HttpRequest::setPath(std::string path);
std::string	HttpRequest::getQueryString();
void		HttpRequest::setQueryString(std::string query);
std::string	HttpRequest::getHttpVersion();
void		HttpRequest::setHttpVersion(std::string httpVersion);
std::string	HttpRequest::getHost();
void		HttpRequest::setHost(std::string host);
std::string	HttpRequest::getConnection();
void		HttpRequest::setConnection(std::string connection);
std::string	HttpRequest::getContentType();
void		HttpRequest::setContentType(std::string contentType);
std::string	HttpRequest::getContentLengh();
void		HttpRequest::setContentLengh(std::string contentLengh);
std::string	HttpRequest::getBody();
void		HttpRequest::setBody(std::string body);

/* ************************************************************************** */